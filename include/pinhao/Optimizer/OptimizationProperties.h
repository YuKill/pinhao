/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimizationProperties.h
 */

#ifndef PINHAO_OPTIMIZATION_PROPERTIES_H
#define PINHAO_OPTIMIZATION_PROPERTIES_H

#include "pinhao/Optimizer/Optimizations.h"
#include "pinhao/Support/Yamlfy.h"
#include "pinhao/Support/Types.h"

#include <stdint.h>
#include <vector>
#include <cassert>

namespace pinhao {
  class OptimizationProperties;
  struct OptimizationArgBase;

  template<>
    class Yamlfy<OptimizationProperties> : public YamlfyTemplateBase<OptimizationProperties> {
      public:
        Yamlfy(const OptimizationProperties *V); 
        void append(YAML::Emitter &Emitter, bool PrintReduced) override;
        void get(const YAML::Node &Node) override;
    };

  template<>
    class Yamlfy<OptimizationArgBase> : public YamlfyTemplateBase<OptimizationArgBase> {
      public:
        Yamlfy(const OptimizationArgBase *V); 
        void append(YAML::Emitter &Emitter, bool PrintReduced) override;
        void get(const YAML::Node &Node) override;
    };

  /**
   * @brief Base class for optimization arguments.
   */
  struct OptimizationArgBase {
    ValueType Type;
    OptimizationArgBase(ValueType Type);
    virtual ~OptimizationArgBase();
  };

  /**
   * @brief An optimization argument of type @a ArgType.
   */
  template <class ArgType>
    struct OptimizationArg : public OptimizationArgBase {
      ArgType Value;
      OptimizationArg(ValueType Type, ArgType Val) :
        OptimizationArgBase(Type), Value(Val) {}
    };

  /**
   * @brief The properties of a optimization, when needed.
   *
   * @details 
   * Includes the number of repetition times of the optimization,
   * the number of arguments and the arguments itself, when needed.
   * The number of arguments are set when this object is created.
   */
  class OptimizationProperties {
    private:

      /// @brief The arguments itself.
      std::vector<OptimizationArgBase> Args;

    public:
      /// @brief Number of repetitions.
      uint64_t Repetition;

      OptimizationProperties();
      OptimizationProperties(Optimization Opt, uint64_t Rep = 1);

      /// @brief Gets a the @a Nth @a OptimizationArg.
      const OptimizationArgBase *getOptimizationArg(uint64_t N);

      /// @brief Sets the @a Nth argument to @a Value.
      template <class ArgType>
        void setArg(int N, ArgType Value) {
          assert(N < Args.size() && "Setting out of bounds argument.");
          OptimizationArg<ArgType> *CastArg = static_cast<OptimizationArg<ArgType>*>(&Args[N]);
          CastArg->Value = Value;
        }

      /// @brief Gets the @a Nth argument.
      template <class ArgType>
        ArgType getArg(uint64_t N) {
          assert(N < Args.size() && "Getting out of bounds argument.");
          OptimizationArg<ArgType> *CastArg = static_cast<OptimizationArg<ArgType>*>(&Args[N]);
          return CastArg->Value;
        }

      /// @brief Gets the type of the @a Nth argument.
      ValueType getArgType(uint64_t N);

      /// @brief Gets the number of arguments.
      uint64_t getNumberOfArguments();

  };

}

#endif
