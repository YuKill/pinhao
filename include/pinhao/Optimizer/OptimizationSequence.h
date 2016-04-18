/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimizationSequence.h
 */

#ifndef PINHAO_OPTIMIZATION_SEQUENCE_H
#define PINHAO_OPTIMIZATION_SEQUENCE_H

#include "pinhao/Optimizer/Optimizations.h"
#include "pinhao/Optimizer/OptimizationInfo.h"
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
      std::vector<OptimizationInfo> Sequence;

      OptimizationSequence() {}

    public:

      /// @brief Generates a @a OptimizationSequence from a specified @a OptimizationSet.
      static std::unique_ptr<OptimizationSequence> generate(OptimizationSet &Set, int Size = -1);

      /// @brief Encapsulates a vector of @a Optimizations into a @a OptimizationSequence.
      static std::unique_ptr<OptimizationSequence> generate(std::vector<Optimization> Sequence);

      /// @brief Generates a random sequence.
      static std::unique_ptr<OptimizationSequence> randomize(uint64_t Size);

      /// @brief Creates a @a OptimizationSequence from a @a YAML::Node.
      static std::unique_ptr<OptimizationSequence> get(YAML::Node &Node);

      /// @brief Populates a @a llvm::legacy::PassManager with the current @a Sequence.
      void populatePassManager(llvm::legacy::PassManager &PM);

      /// @brief Gets the @a OptimizationInfo of the @a Nth element of the sequence.
      OptimizationInfo getOptimization(uint64_t N);

      /// @brief Gets the size of the sequence.
      uint64_t size();

      /// @brief Populates a @a llvm::legacy::FunctionPassManager with the current @a Sequence.
      /// @details Note that all the optimizations must be for functions (PassKind < 4).
      void populateFunctionPassManager(llvm::legacy::FunctionPassManager &FPM);

      /// @brief Prints the sequence at @a Out.
      void print(std::ostream &Out = std::cout);

      typedef std::vector<OptimizationInfo>::iterator SequenceIterator;
      SequenceIterator begin() { return Sequence.begin(); }
      SequenceIterator end() { return Sequence.end(); }

      friend class Yamlfy<OptimizationSequence>;
  
  };

}

#endif


