/*-------------------------- PINHAO project --------------------------*/

/**
 * @file SProfWrapper.h
 */

#ifndef PINHAO_SPROF_WRAPPER_H
#define PINHAO_SPROF_WRAPPER_H

#include "llvm/IR/LegacyPassManager.h"

#include "sprof/StaticModuleCost.h"

#include <map>

namespace pinhao {

  /**
   * @brief This class is a wrapper of the @a StaticProfiler tool.
   *
   * @details
   * It calculates, statically, the cost of each @a llvm::Function. For
   * the @a llvm::Module, it sums up all its @a llvm::Function costs.
   */
  class SProfWrapper {
    public:
      typedef std::shared_ptr<llvm::StaticModuleCost> StaticModuleCostPtr;
      typedef std::map<llvm::Function*, double> CostMap;

    private:
      /// @brief Creates the @a StaticProfiler main @a llvm::Pass.
      static llvm::Pass *getSProfPass(StaticModuleCostPtr);

      /// @brief Runs the @a llvm::Pass, and returns a struct with its results.
      static StaticModuleCostPtr runSProfPass(llvm::Module&);

    public:
      /// @brief Gets a @a llvm::Function cost.
      static double getFunctionCost(llvm::Module&, llvm::Function&);

      /// @brief Returns the sum of all @ llvm::Functions from the @a llvm::Module.
      static double getModuleCost(llvm::Module&);

      /// @brief Returns the cost of each @a llvm::Function.
      static CostMap getAllFunctionsCost(llvm::Module&);
  
  };
  
}

#endif


