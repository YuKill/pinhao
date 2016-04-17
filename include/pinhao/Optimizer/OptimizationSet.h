/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimizationSet.h
 */

#ifndef PINHAO_OPTIMIZATION_SET_H
#define PINHAO_OPTIMIZATION_SET_H

#include "pinhao/Optimizer/Optimizations.h"
#include "pinhao/Optimizer/OptimizationInfo.h"
#include "pinhao/Optimizer/OptimizationSequence.h"

#include <string>
#include <vector>
#include <map>

namespace pinhao {

  /**
   * @brief Holds all enabled optimizations as well as the number of repetition
   * of each one.
   */
  class OptimizationSet {
    private:
      /// @brief The enabled optimizations paired with its repetition number.
      std::vector<std::pair<OptimizationInfo, uint64_t>> EnabledOptimizations;

    public:
      OptimizationSet();
      ~OptimizationSet();

      /// @brief The default sequence in which it will be compiled.
      OptimizationSequence *DefaultSequence;

      /// @brief Constructs an @a OptimizationSet from a given @a OptimizationSequence.
      static std::unique_ptr<OptimizationSet> getFromSequence(OptimizationSequence Seq);

      /// @brief Returns true if the @a Optimization @a Opt has already been enabled.
      bool hasEnabled(Optimization Opt);

      /// @brief Removes @a Opt if enabled.
      void disableOptimization(Optimization Opt);
      /// @brief Enables the @a Opt with default properties.
      void enableOptimization(Optimization Opt, uint64_t Repetition = 1);
      /// @brief Enables a optimization with specific @a OptimizationInfo.
      void enableOptimization(OptimizationInfo Info, uint64_t Repetition = 1);
      /// @brief Enables the optimizations with default properties.
      void enableOptimizations(std::vector<Optimization> Opts);

      /// @brief Enables all optimizations with default properties.
      void enableAllOptimizations();
      /// @brief Enables all module optimizations with default properties.
      void enableModuleOptimizations();
      /// @brief Enables all function optimizations with default properties.
      void enableFunctionOptimizations();

      /// @brief Generates a random sequence (it will be the default sequence),
      /// in order to compile.
      void generateRandomSequenceIfNone();

      /// @brief Returns the number of enabled optimizations.
      uint64_t size();

      typedef std::vector<std::pair<OptimizationInfo, uint64_t>>::iterator SetIterator;
      SetIterator begin() { return EnabledOptimizations.begin(); }
      SetIterator end() { return EnabledOptimizations.end(); }
  };

}

#endif


