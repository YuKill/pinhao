/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimizationProperties.h
 */

#ifndef PINHAO_OPTIMIZATION_PROPERTIES_H
#define PINHAO_OPTIMIZATION_PROPERTIES_H

#include "pinhao/Support/Types.h"

#include <stdint.h>
#include <vector>
#include <cassert>

namespace pinhao {
  enum class Optimization;

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

      /// @brief Sets the @a N argument to @a Value.
      template <class ArgType>
        void setArg(int N, ArgType Value) {
          assert(N < Args.size() && "Setting out of bounds argument.");
          OptimizationArg<ArgType> *CastArg = static_cast<OptimizationArg<ArgType>*>(&Args[N]);
          CastArg->Value = Value;
        }

      template <class ArgType>
        ArgType getArg(uint64_t N) {
          assert(N < Args.size() && "Getting out of bounds argument.");
          OptimizationArg<ArgType> *CastArg = static_cast<OptimizationArg<ArgType>*>(&Args[N]);
          return CastArg->Value;
        }

      /// @brief Gets the number of arguments.
      uint64_t getNumberOfArguments();

  };

}

#endif


