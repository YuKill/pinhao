/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimizationSequence.h
 */

#ifndef PINHAO_OPTIMIZATION_SEQUENCE_H
#define PINHAO_OPTIMIZATION_SEQUENCE_H

#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"

#include <vector>

namespace pinhao {
  class OptimizationSequence;
  class OptimizationSet;
  enum class Optimization;

  /// @brief Applies the @a OptimizationSequence to a module.
  /// @return The optimized module.
  llvm::Module *optimize(llvm::Module &Module, OptimizationSequence &Sequence);

  /// @brief Applies the @a OptimizationSequence to a function.
  /// @return The optimized function's module.
  llvm::Module *optimize(llvm::Module &Module, llvm::Function &Function, OptimizationSequence &Sequence);

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

      typedef std::vector<Optimization>::iterator SequenceIterator;
      SequenceIterator begin() { return Sequence.begin(); }
      SequenceIterator end() { return Sequence.end(); }
  
  };

}

#endif


