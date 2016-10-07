/*-------------------------- PINHAO project --------------------------*/

/**
 * @file SimpleGrammarEvolution.cpp
 */

#include "pinhao/MachineLearning/GrammarEvolution/SProfSimpleGrammarEvolution.h"
#include "pinhao/MachineLearning/GrammarEvolution/Candidate.h"
#include "pinhao/MachineLearning/GrammarEvolution/Formula.h"

#include "pinhao/Optimizer/OptimizationSet.h"
#include "pinhao/PerformanceAnalyser/PAPIWrapper.h"
#include "pinhao/PerformanceAnalyser/SProfWrapper.h"
#include "pinhao/Support/YamlOptions.h"

using namespace pinhao;

/*
 * -------------------------------------
 *  Class: SProfSimpleGrammarEvolution
 */
static config::YamlOpt<std::string> SequenceFile
("sequence", "The file which contains a sequence of optimization.", false, ".sequence.yaml");

SProfSimpleGrammarEvolution::SProfSimpleGrammarEvolution(std::shared_ptr<llvm::Module> Module, std::string KBFilename,
    double EvolveProb, double MaxEvolutionRate, double MutateProb) : 
  SimpleGrammarEvolution(Module, KBFilename, EvolveProb, MaxEvolutionRate, MutateProb) {
  }

void pinhao::SProfSimpleGrammarEvolution::run(int CandidatesNumber, int GenerationsNumber, 
    std::shared_ptr<FeatureSet> Set) {
  typedef std::pair<double, Candidate> RankingPair;
  typedef std::greater<RankingPair> DecendantOrder;

  getSequence(SequenceFile.get());

  SimpleEvolution EvolutionStrategy(MutateProbability, Set.get());

  addPreDefinedDecisionPoints();
  importKnowledgeBase();

  std::set<RankingPair, DecendantOrder> Ranking;

  double BaseLine = SProfWrapper::getModuleCost(*Module);
  uint64_t RealBaseLine = PAPIWrapper::getTotalCycles(*Module, Argv).second;

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
        double Cost = SProfWrapper::getModuleCost(*Compiled);
        if (Cost > 0.01) {
          double SpeedUp = BaseLine / Cost;
          RankingTmp.insert(std::make_pair(SpeedUp, C));
          std::cerr << "SpeedUp: " << SpeedUp << std::endl;
          continue;
        }
      }
      std::cerr << "SpeedUp: 0.3" << std::endl;
      RankingTmp.insert(std::make_pair(0.3, C));
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

  auto Compiled = compileWithCandidate(Module.get(), const_cast<Candidate&>((*Ranking.begin()).second), Set.get());
  assert(Compiled && "Error while compiling best candidate.");
  auto PAPIPair = PAPIWrapper::getTotalCycles(*Compiled, Argv);
  assert(!PAPIPair.first && "Error while compiling best candidate.");

  std::cerr << "BestPred:" << (*Ranking.begin()).first << std::endl;
  std::cerr << "Best:" << (double) RealBaseLine / PAPIPair.second << std::endl;
  std::cerr << "Cycles:" << PAPIPair.second << std::endl;
  stop();
}
