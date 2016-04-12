/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimizationProperties.cpp
 */

#include "pinhao/Optimizer/OptimizationProperties.h"
#include "pinhao/Optimizer/OptimizationSet.h"

using namespace pinhao;

/*
 * ----------------------------------=
 * Struct: OptimizationArgBase
 */
OptimizationArgBase::OptimizationArgBase(ValueType Type) : Type(Type) {

}

/*
 * ----------------------------------=
 * Struct: OptimizationProperties
 */
OptimizationProperties::OptimizationProperties() {

}

OptimizationProperties::OptimizationProperties(Optimization Opt, uint64_t Rep) : Repetition(Rep) {
  switch (Opt) {
    case Optimization::gvn:
      Args.push_back(OptimizationArg<bool>(ValueType::Bool, true));
      break;

    case Optimization::jumpThreading:
      Args.push_back(OptimizationArg<int>(ValueType::Int, -1));
      break;

    case Optimization::simplifycfg:
      Args.push_back(OptimizationArg<int>(ValueType::Int, -1));
      break;

    case Optimization::loopRotate:
      Args.push_back(OptimizationArg<int>(ValueType::Int, -1));
      break;

    case Optimization::loopUnswitch:
      Args.push_back(OptimizationArg<bool>(ValueType::Bool, false));
      break;

    case Optimization::loopUnroll:
      Args = std::vector<OptimizationArgBase> {
        OptimizationArg<int>(ValueType::Int, -1),
          OptimizationArg<int>(ValueType::Int, -1),
          OptimizationArg<int>(ValueType::Int, -1),
          OptimizationArg<int>(ValueType::Int, -1)
      };
      break;

    case Optimization::scalarrepl:
      Args = std::vector<OptimizationArgBase> {
        OptimizationArg<int>(ValueType::Int, -1),
          OptimizationArg<bool>(ValueType::Bool, true),
          OptimizationArg<int>(ValueType::Int, -1),
          OptimizationArg<int>(ValueType::Int, -1),
          OptimizationArg<int>(ValueType::Int, -1)
      };
      break;

    default:
      break;
  };
}

uint64_t OptimizationProperties::getNumberOfArguments() {
  return Args.size();
}

