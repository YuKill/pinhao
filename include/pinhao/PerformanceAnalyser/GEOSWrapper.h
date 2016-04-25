/*-------------------------- PINHAO project --------------------------*/

/**
 * @file GEOSWrapper.h
 */

#ifndef PINHAO_GEOS_WRAPPER_H
#define PINHAO_GEOS_WRAPPER_H

#include "llvm/IR/Module.h"

#include "geos/ProfileModule/ProfileModule.h"

#include <vector>
#include <memory>

namespace pinhao {

  /**
   * @brief This class is actually a wrapper to a set of functions of the
   * GEOS framework.
   *
   * @details
   * The functions that calculates the costs returns a @a std::vector<double>
   * with all GEOS' costs. They are:
   *
   *   - RegisterUse
   *   - InstructionCache
   *   - StaticInstruction
   *   - TTIInstruction
   *   - Branch
   *   - Call
   *   - NonArchSensitive
   *   - ArchSensitive
   */
  class GEOSWrapper {
    private:
      /// @brief Gets all GEOS' measurements of a @a ProfileModule.
      static std::vector<double> getAnalysisCost(std::shared_ptr<ProfileModule>);

      /// @brief Propagates the frequencies stored in the @a ProfileModule
      /// to the instructions.
      static void propagateToInstructions(ProfileModule&);

    public:
      /// @brief Loads the callcost provided.
      static void loadCallCostFile(llvm::Module&);

      /// @brief Runs the @a llvm::Module inside the @a ProfileModule, in order
      /// to get the @a BasicBlock frequencies.
      static void getFrequencies(std::shared_ptr<ProfileModule>);

      /// @brief Repairs the frequencies (from the instructions), and calculates
      /// all GEOS' costs.
      static std::vector<double> repairAndAnalyse(llvm::Module&);

      /// @brief Gets the frequencies by running the @a llvm::Module, and calculates
      /// all GEOS' costs.
      static std::vector<double> getFrequenciesAndAnalyse(llvm::Module&);
  
  };


}

#endif


