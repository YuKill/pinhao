/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimizationSequence.h
 */

#ifndef PINHAO_OPTIMIZATION_SEQUENCE_H
#define PINHAO_OPTIMIZATION_SEQUENCE_H

#include "pinhao/Optimizer/Optimizations.h"
#include "pinhao/Support/Yamlfy.h"

#include "llvm/IR/LegacyPassManager.h"

#include <vector>
#include <iostream>

namespace pinhao {
  class OptimizationSet;
  class OptimizationSequence;

  template<>
    class Yamlfy<OptimizationSequence> : public YamlfyTemplateBase<OptimizationSequence> {
      public:
        Yamlfy(const OptimizationSequence *V); 
        void append(YAML::Emitter &Emitter, bool PrintReduced) override;
        void get(const YAML::Node &Node) override;
    };

  /**
   * @brief Contains a sequence for a specific @a OptimizationSet.
   */
  class OptimizationSequence {
    private:
      /// @brief The sequence of optimizations itself.
      std::vector<Optimization> Sequence;

      /// @brief A reference to the set from where it was generated.
      std::shared_ptr<OptimizationSet> Set;

      /// @brief Creates the specific @a llvm::Pass of the @a Optimization.
      llvm::Pass *getPass(Optimization Opt); 

      OptimizationSequence() {}

    public:

      /// @brief Generates a @a OptimizationSequence from a specified @a OptimizationSet.
      static std::unique_ptr<OptimizationSequence> generate(std::shared_ptr<OptimizationSet> Set);

      /// @brief Encapsulates a vector of @a Optimizations into a @a OptimizationSequence.
      static std::unique_ptr<OptimizationSequence> generate(std::vector<Optimization> Sequence);

      /// @brief Generates a random sequence.
      static std::unique_ptr<OptimizationSequence> randomize(uint64_t Size);

      /// @brief Populates a @a llvm::legacy::PassManager with the current @a Sequence.
      void populatePassManager(llvm::legacy::PassManager &PM);

      /// @brief Populates a @a llvm::legacy::FunctionPassManager with the current @a Sequence.
      /// @details Note that all the optimizations must be for functions (PassKind < 4).
      void populateFunctionPassManager(llvm::legacy::FunctionPassManager &FPM);

      /// @brief Prints the sequence at the standard output.
      void print();
      /// @brief Prints the sequence at @a Out.
      void print(std::ostream &Out);

      typedef std::vector<Optimization>::iterator SequenceIterator;
      SequenceIterator begin() { return Sequence.begin(); }
      SequenceIterator end() { return Sequence.end(); }

      friend class Yamlfy<OptimizationSequence>;
  
  };

}

#endif


