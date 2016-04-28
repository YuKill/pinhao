/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimizationInfo.h
 */

#ifndef PINHAO_OPTIMIZATION_INFO_H
#define PINHAO_OPTIMIZATION_INFO_H

#include "pinhao/Optimizer/Optimizations.h"
#include "pinhao/Support/Types.h"

namespace pinhao {

  /**
   * @brief Base class for optimization arguments.
   */
  struct OptimizationArgBase {
    ValueType Type;
    OptimizationArgBase(ValueType Type) : Type(Type) {}
    virtual ~OptimizationArgBase() {}
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

  class OptimizationInfo {
    private:
      Optimization Opt;

      /// @brief The arguments of this optimization.
      std::vector<OptimizationArgBase*> Args;

      /// @brief Copies the content of @a Info to @a this object.
      void copy(const OptimizationInfo &Info);

    public:
      ~OptimizationInfo();

      OptimizationInfo();
      OptimizationInfo(Optimization Opt);
      OptimizationInfo(std::string OptName);
      OptimizationInfo(const OptimizationInfo &Info);

      /// @brief Gets the name of the optimization.
      std::string getName() const;
      /// @brief Gets the optimization.
      Optimization getOptimization() const;
      /// @brief Creates a @a llvm::Pass corresponding to this optimization.
      llvm::Pass *createPass();

      /// @brief Gets a the @a Nth @a OptimizationArg.
      const OptimizationArgBase *getOptimizationArg(uint64_t N) const;

      /// @brief Sets the @a Nth argument to @a Value.
      template <class ArgType>
        void setArg(int N, ArgType Value) const {
          assert(N < Args.size() && "Setting out of bounds argument.");
          OptimizationArg<ArgType> *CastArg = static_cast<OptimizationArg<ArgType>*>(Args[N]);
          CastArg->Value = Value;
        }

      /// @brief Gets the @a Nth argument.
      template <class ArgType>
        ArgType getArg(uint64_t N) const {
          assert(N < Args.size() && "Getting out of bounds argument.");
          OptimizationArg<ArgType> *CastArg = static_cast<OptimizationArg<ArgType>*>(Args[N]);
          return CastArg->Value;
        }

      /// @brief Gets the type of the @a Nth argument.
      ValueType getArgType(uint64_t N) const;

      /// @brief Gets the number of arguments.
      uint64_t getNumberOfArguments() const;

      /// @brief Copy assignment operator.
      OptimizationInfo &operator=(const OptimizationInfo &Info);

      typedef std::vector<OptimizationArgBase*>::iterator ArgsIterator;
      ArgsIterator begin() { return Args.begin(); }
      ArgsIterator end() { return Args.end(); }

  };

}

#endif


