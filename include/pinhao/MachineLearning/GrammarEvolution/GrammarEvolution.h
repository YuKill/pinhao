/*-------------------------- PINHAO project --------------------------*/

/**
 * @file GrammarEvolution.h
 */

#ifndef PINHAO_GRAMMAR_EVOLUTION_H
#define PINHAO_GRAMMAR_EVOLUTION_H

#include "pinhao/MachineLearning/GrammarEvolution/Evolution.h"
#include "pinhao/Optimizer/Optimizations.h"
#include "pinhao/Support/SerialSet.h"
#include "pinhao/Support/YAMLWrapper.h"
#include "pinhao/Support/Types.h"

#include <vector>

namespace pinhao {

  struct DecisionPoint {
    std::string Name;
    ValueType Type;

    DecisionPoint() {}
    DecisionPoint(std::string Name, ValueType Type) : Name(Name), Type(Type) {}

    bool operator<(const DecisionPoint &Rhs) const {
      return Name < Rhs.Name;
    }   

  };

  template <class T>
    class GrammarEvolution {
      protected:
        std::shared_ptr<llvm::Module> Module;
        std::string KnowledgeBaseFile;
        double EvolveProbability;
        double MaxEvolutionRate;
        double MutateProbability;

        std::vector<DecisionPoint> DecisionPoints;
        SerialSet<T> KnowledgeBase;

        virtual void addPreDefinedDecisionPoints();
        virtual void importKnowledgeBase();
        virtual void exportKnowledgeBase();

        virtual llvm::Module *compileWithCandidate(llvm::Module*, T&, FeatureSet*) = 0;

      public:
        virtual ~GrammarEvolution();
        GrammarEvolution(std::shared_ptr<llvm::Module> /* Module */, std::string /* KBFilename */ = "config.yaml",
            double /* EvolveProb */ = 0.2, double /* MaxEvolutionRate */ = 0.3, double /* MutateProb */ = 0.3);

        virtual void run(int /* CandidatesNumber */, int /* GenerationsNumber */, 
            std::shared_ptr<FeatureSet>) = 0;
        virtual void stop() = 0;

    };

}


template <class T>
pinhao::GrammarEvolution<T>::~GrammarEvolution() {

}

template <class T>
pinhao::GrammarEvolution<T>::GrammarEvolution(std::shared_ptr<llvm::Module> Module, std::string KBFilename,
    double EvolveProb, double MaxEvolutionRate, double MutateProb) : 
  Module(Module), KnowledgeBaseFile(KBFilename), 
  EvolveProbability(EvolveProb), MaxEvolutionRate(MaxEvolutionRate), MutateProbability(MutateProb) {
  }

template <class T>
void pinhao::GrammarEvolution<T>::addPreDefinedDecisionPoints() {
  for (auto OptName : Optimizations) {
    DecisionPoint DP(OptName, ValueType::Bool); 
    DecisionPoints.push_back(DP);
  }
}

template <class T>
void pinhao::GrammarEvolution<T>::importKnowledgeBase() {
  auto Node = YAMLWrapper::loadFile(KnowledgeBaseFile);
  YAMLWrapper::fill(KnowledgeBase, Node);
}

template <class T>
void pinhao::GrammarEvolution<T>::exportKnowledgeBase() {
  std::ofstream Of(KnowledgeBaseFile);
  YAMLWrapper::print(KnowledgeBase, Of);
}

#endif
