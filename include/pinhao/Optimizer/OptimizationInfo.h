/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimizationInfo.h
 */

#ifndef PINHAO_OPTIMIZATION_INFO_H
#define PINHAO_OPTIMIZATION_INFO_H

#include "pinhao/Optimizer/Optimizations.h"
#include "pinhao/Support/Yamlfy.h"
#include "pinhao/Support/Types.h"

namespace pinhao {
  class OptimizationInfo;
  struct OptimizationArgBase;

  template<>
    class Yamlfy<OptimizationInfo> : public YamlfyTemplateBase<OptimizationInfo> {
      public:
        Yamlfy(const OptimizationInfo *V); 
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
      std::vector<std::shared_ptr<OptimizationArgBase>> Args;

    public:
      ~OptimizationInfo();
      OptimizationInfo() {}
      OptimizationInfo(Optimization Opt);
      OptimizationInfo(std::string OptName);

      /// @brief Gets the name of the optimization.
      std::string getName();
      /// @brief Gets the optimization.
      Optimization getOptimization() const;
      /// @brief Creates a @a llvm::Pass corresponding to this optimization.
      llvm::Pass *createPass();

      /// @brief Gets a the @a Nth @a OptimizationArg.
      const OptimizationArgBase *getOptimizationArg(uint64_t N);

      /// @brief Sets the @a Nth argument to @a Value.
      template <class ArgType>
        void setArg(int N, ArgType Value) {
          assert(N < Args.size() && "Setting out of bounds argument.");
          OptimizationArg<ArgType> *CastArg = static_cast<OptimizationArg<ArgType>*>(Args[N].get());
          CastArg->Value = Value;
        }

      /// @brief Gets the @a Nth argument.
      template <class ArgType>
        ArgType getArg(uint64_t N) {
          assert(N < Args.size() && "Getting out of bounds argument.");
          OptimizationArg<ArgType> *CastArg = static_cast<OptimizationArg<ArgType>*>(Args[N].get());
          return CastArg->Value;
        }

      /// @brief Gets the type of the @a Nth argument.
      ValueType getArgType(uint64_t N);

      /// @brief Gets the number of arguments.
      uint64_t getNumberOfArguments();

  };

}

#endif


