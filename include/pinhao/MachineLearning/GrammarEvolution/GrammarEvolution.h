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

  /**
   * @brief It represents each decision the given compiler has to make.
   * 
   * @details
   * For example, there may be an decision point with each optimization name, and
   * with type @a ValueType::Bool. Which means that this is the point that decides
   * whether the given optimization is to be used or not.
   */
  struct DecisionPoint {
    std::string Name;
    ValueType Type;

    DecisionPoint() {}
    DecisionPoint(std::string Name, ValueType Type) : Name(Name), Type(Type) {}

    bool operator<(const DecisionPoint &Rhs) const {
      return Name < Rhs.Name;
    }   

  };

  /**
   * @brief The base @a GrammarEvolution class.
   * 
   * @details
   * It contains the basic information and implementations that a @a GrammarEvolution
   * algorithm needs. As default, it starts when the @a run function is executed, and
   * it ends with the execution of the @a stop function.
   */
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

        /**
         * @brief Adds some pre-defined decision points.
         *
         * @details
         * This base class adds only decision points related to whether one optimization
         * is going to be used or not.
         */
        virtual void addPreDefinedDecisionPoints();
        /// @brief Imports the knowledge base that is inside the file @a KnowledgeBaseFile.
        virtual void importKnowledgeBase();
        /// @brief Exports the knowledge base to the file @a KnowledgeBaseFile.
        virtual void exportKnowledgeBase();

        /// @brief Depends on the @a DecisionPoint's added. As it changes, the way to compile
        /// the module also changes.
        virtual llvm::Module *compileWithCandidate(llvm::Module*, T&, FeatureSet*) = 0;

      public:
        virtual ~GrammarEvolution();
        GrammarEvolution(std::shared_ptr<llvm::Module> /* Module */, std::string /* KBFilename */ = "config.yaml",
            double /* EvolveProb */ = 0.2, double /* MaxEvolutionRate */ = 0.3, double /* MutateProb */ = 0.3);

        /// @brief By default, is the function that starts the execution of the algorithm.
        virtual void run(int /* CandidatesNumber */, int /* GenerationsNumber */, 
            std::shared_ptr<FeatureSet>) = 0;
        /// @brief By default, is the function that ends the execution of the algorithm.
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
