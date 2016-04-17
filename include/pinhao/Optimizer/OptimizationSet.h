/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimizationSet.h
 */

#ifndef PINHAO_OPTIMIZATION_SET_H
#define PINHAO_OPTIMIZATION_SET_H

#include "pinhao/Optimizer/Optimizations.h"
#include "pinhao/Optimizer/OptimizationProperties.h"
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
      /// @brief The enabled optimizations paired with its properties.
      std::map<Optimization, OptimizationProperties> EnabledOptimizations;

    public:
      OptimizationSet();
      ~OptimizationSet();

      /// @brief A generated sequence used by the @a optimize method.
      OptimizationSequence *DefaultSequence;

      /// @brief Constructs an @a OptimizationSet from a given @a OptimizationSequence.
      static std::unique_ptr<OptimizationSet> getFromSequence(OptimizationSequence Seq);

      /// @brief Returns true if the @a Optimization @a Opt has already enabled.
      bool hasEnabled(Optimization Opt);

      /// @brief Removes @a Opt if enabled.
      void disableOptimization(Optimization Opt);
      /// @brief Enables the @a Opt with default properties.
      void enableOptimization(Optimization Opt);
      /// @brief Enables the @a Opt with some given property.
      void enableOptimization(Optimization Opt, OptimizationProperties Prop);
      /// @brief Enables the optimizations with default properties.
      void enableOptimizations(std::vector<Optimization> Opts);

      /// @brief Enables all optimizations with default properties.
      void enableAllOptimizations();
      /// @brief Enables all module optimizations with default properties.
      void enableModuleOptimizations();
      /// @brief Enables all function optimizations with default properties.
      void enableFunctionOptimizations();

      /// @brief Sets the properties of the optimization @a Opt.
      void setOptimizationProperties(Optimization Opt, OptimizationProperties Prop);
      /// @brief Gets a copy of the properties of the optimization @a Opt.
      OptimizationProperties getOptimizationProperties(Optimization Opt);

      /// @brief Generates a random sequence for this set (if there is none), and sets it to default.
      void generateRandomSequenceIfNone();

      /// @brief Returns the number of enabled optimizations.
      uint64_t size();

      typedef std::map<Optimization, OptimizationProperties>::iterator SetIterator;
      SetIterator begin() { return EnabledOptimizations.begin(); }
      SetIterator end() { return EnabledOptimizations.end(); }
  };

}

#endif


