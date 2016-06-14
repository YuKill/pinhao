/*-------------------------- PINHAO project --------------------------*/

/**
 * @file SimpleGrammarEvolution.cpp
 */

#include "pinhao/MachineLearning/GrammarEvolution/SimpleGrammarEvolution.h"
#include "pinhao/MachineLearning/GrammarEvolution/Candidate.h"
#include "pinhao/MachineLearning/GrammarEvolution/Formula.h"

#include "pinhao/Optimizer/OptimizationSet.h"
#include "pinhao/PerformanceAnalyser/PAPIWrapper.h"

#include <algorithm>

using namespace pinhao;

/*
 * -------------------------------------
 *  Class: SimpleEvolution
 */
class SimpleEvolution : public Evolution {
  private:
    double M;

  public:
    SimpleEvolution(double M, FeatureSet *Set) : Evolution(Set), M(M) {}

    void mutate(std::unique_ptr<FormulaBase>*);
    void evolve(std::vector<std::unique_ptr<FormulaBase>*>) override; 
    void evolve(std::pair<std::string, std::string>&) override; 
    void evolve(int&) override;
    void evolve(bool&) override;
};

void SimpleEvolution::mutate(std::unique_ptr<FormulaBase>* Form) {
  ValueType Type = (*Form)->getType();
  (*Form).reset(generateFormula(Set, Type).release());
}

void SimpleEvolution::evolve(std::vector<std::unique_ptr<FormulaBase>*> Vector) {
  int Chosen = UniformRandom::getRandomInt(0, Vector.size()-1);
  double Die = UniformRandom::getRandomReal();
  if (Die <= M) mutate(Vector[Chosen]);
  else (*Vector[Chosen])->evolve(this);
} 

void SimpleEvolution::evolve(std::pair<std::string, std::string> &Pair) {
  ValueType Type = Set->getFeatureType(Pair.first);
  auto Total = Set->count(Type);
  auto Index = UniformRandom::getRandomInt(0, Total-1);
  Pair = *Set->get(Index, Type);
} 

void SimpleEvolution::evolve(int &Value) {
  int N = UniformRandom::getRandomInt(0, 2 * Value);
  Value = N;
}

void SimpleEvolution::evolve(bool &Value) {
  Value = !Value;
}

/*
 * -------------------------------------
 *  Class: SimpleGrammarEvolution
 */
static std::string SequenceFile = ".sequence-" + std::to_string(time(0)) + ".yaml";

SimpleGrammarEvolution::~SimpleGrammarEvolution() {

}

SimpleGrammarEvolution::SimpleGrammarEvolution(std::shared_ptr<llvm::Module> Module, std::string KBFilename,
    double EvolveProb, double MaxEvolutionRate, double MutateProb) : 
  GrammarEvolution(Module, KBFilename, EvolveProb, MaxEvolutionRate, MutateProb) {

  }

llvm::Module *pinhao::SimpleGrammarEvolution::
compileWithCandidate(llvm::Module *Module, Candidate &C, FeatureSet *Set) {

  OptimizationSet OptSet;
  OptimizationSequence OptSequence;

  for (auto &Pair : C) {
    if (std::find(Optimizations.begin(), Optimizations.end(), Pair.first.Name) == Optimizations.end())
      continue;
    FormulaBase *Form = Pair.second;
    std::string OptName = Pair.first.Name;

    Form->solveFor(Set);
    bool EnableOpt = getFormulaValue<bool>(Form);
    if (EnableOpt)
      OptSet.addOptimization(getOptimization(OptName));
  }

  for (auto O : Sequence) {
    Optimization Opt = (Optimization) O;
    if (!OptSet.hasEnabled(Opt)) continue;

    OptimizationInfo Info = OptSet.getOptimizationInfo(Opt); 
    OptSequence.push_back(Info);
  }

  return applyOptimizations(*Module, &OptSequence);
}

void pinhao::SimpleGrammarEvolution::run(int CandidatesNumber, int GenerationsNumber, 
    std::shared_ptr<FeatureSet> Set) {
  typedef std::pair<double, Candidate> RankingPair;
  typedef std::greater<RankingPair> DecendantOrder;

  getSequence(SequenceFile);

  SimpleEvolution EvolutionStrategy(MutateProbability, Set.get());

  addPreDefinedDecisionPoints();
  importKnowledgeBase();

  std::set<RankingPair, DecendantOrder> Ranking;

  uint64_t BaseLine = PAPIWrapper::getTotalCycles(*Module, Argv).second;

  for (int I = 0; I < GenerationsNumber; ++I) {
    std::set<RankingPair, DecendantOrder> RankingTmp;

    std::set<Candidate, CompareByScore> ScoreSet;
    for (auto &C : KnowledgeBase)
      ScoreSet.insert(C); 

    int Count = 0;
    std::vector<Candidate> BestCandidates;
    for (auto &C : ScoreSet) {
      if (Count >= CandidatesNumber) break;
      Candidate *Clone = C.clone();
      BestCandidates.push_back(*Clone); 
      ++Count;

      delete Clone;
    }

    if (BestCandidates.size() > 0) {
      for (auto &C : BestCandidates) {
        double EvolveDie = UniformRandom::getRandomReal();
        if (EvolveDie < EvolveProbability) {
          C.evolve(MaxEvolutionRate, &EvolutionStrategy);
        }
      }

      BestCandidates.push_back(*ScoreSet.begin());
    } else {
      BestCandidates = std::vector<Candidate>(CandidatesNumber, Candidate()); 
    }

    for (auto &C : BestCandidates) {
      C.generateMissing(DecisionPoints, Set.get());

      auto Compiled = compileWithCandidate(Module.get(), C, Set.get());
      if (Compiled) {
        auto PAPIPair = PAPIWrapper::getTotalCycles(*Compiled, Argv); 
        if (PAPIPair.first == 0) {
          double SpeedUp = (double) BaseLine / PAPIPair.second;
          RankingTmp.insert(std::make_pair(SpeedUp, C));
          std::cerr << "SpeedUp: " << SpeedUp << std::endl;
          continue;
        }
      }
      std::cerr << "SpeedUp: 0.7" << std::endl;
      RankingTmp.insert(std::make_pair(0.7, C));
    }

    for (auto RPair : RankingTmp) {
      Candidate C = RPair.second;

      if (KnowledgeBase.has(C)) {
        C.Score = ((C.Count * C.Score) + RPair.first) / (C.Count + 1); 
        ++C.Count;
      } else {
        C.Score = RPair.first;
        C.Count = 1;
      }

      KnowledgeBase.update(C);
      Ranking.insert(RPair);
    }

  }

  std::cerr << "Best: " << (*Ranking.begin()).first << std::endl;
  stop();
}

void pinhao::SimpleGrammarEvolution::stop() {
  exportKnowledgeBase();
}
