/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimizationSequence.h
 */

#ifndef PINHAO_OPTIMIZATION_SEQUENCE_H
#define PINHAO_OPTIMIZATION_SEQUENCE_H

#include "pinhao/Optimizer/Optimizations.h"
#include "pinhao/Optimizer/OptimizationInfo.h"

#include "llvm/IR/LegacyPassManager.h"

#include <vector>
#include <iostream>

namespace pinhao {
  class OptimizationSet;

  /**
   * @brief Contains a sequence for a specific @a OptimizationSet.
   */
  class OptimizationSequence : public std::vector<OptimizationInfo> {
    public:
      OptimizationSequence() {}

      /// @brief Generates a @a OptimizationSequence from a specified @a OptimizationSet.
      static std::unique_ptr<OptimizationSequence> generate(OptimizationSet &Set, int Size = -1);

      /// @brief Encapsulates a vector of @a Optimizations into a @a OptimizationSequence.
      static std::unique_ptr<OptimizationSequence> generate(std::vector<Optimization> Sequence);

      /// @brief Generates a random sequence.
      static std::unique_ptr<OptimizationSequence> randomize(uint64_t Size);

      /// @brief Populates a @a llvm::legacy::PassManager with the current @a Sequence.
      void populatePassManager(llvm::legacy::PassManager &PM);

      /// @brief Gets the @a OptimizationInfo of the @a Nth element of the sequence.
      OptimizationInfo getOptimization(uint64_t N);

      /// @brief Populates a @a llvm::legacy::FunctionPassManager with the current @a Sequence.
      /// @details Note that all the optimizations must be for functions (PassKind < 4).
      void populateFunctionPassManager(llvm::legacy::FunctionPassManager &FPM);

  };

}

#endif


