/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimizationProperties.cpp
 */

#include "pinhao/Optimizer/Optimizations.h"
#include "pinhao/Optimizer/OptimizationProperties.h"
#include "pinhao/Optimizer/OptimizationSet.h"

using namespace pinhao;

/*
 * ----------------------------------=
 * Class: Yamlfy<OptimizationProperties>
 */
Yamlfy<OptimizationProperties>::Yamlfy(const OptimizationProperties *V) : 
  YamlfyTemplateBase(V) {

  }

void Yamlfy<OptimizationProperties>::append(YAML::Emitter &Emitter, bool PrintReduced) {
  Emitter << YAML::BeginMap;

  Emitter << YAML::Key << "rep";
  Emitter << YAML::Value << std::to_string(Value->Repetition);

  if (Value->getNumberOfArguments() > 0) {
    Emitter << YAML::Key << "args";
    Emitter << YAML::Value << YAML::BeginSeq;
    for (int I = 0, E = Value->getNumberOfArguments(); I < E; ++I)
      Yamlfy<OptimizationArgBase>(Value->getOptimizationArg(I)).append(Emitter, false);
    Emitter << YAML::EndSeq;
  }   

  Emitter << YAML::EndMap;
}

void Yamlfy<OptimizationProperties>::get(const YAML::Node &Node) {
  Value->Repetition = Node["rep"].as<int>();
  if (Node["args"])
    for (uint64_t I = 0, E = Value->getNumberOfArguments(); I < E; ++I)
      Yamlfy<OptimizationArgBase>(Value->getOptimizationArg(I)).get(Node[I]);
}

/*
 * ----------------------------------=
 * Class: Yamlfy<OptimizationArgBase>
 */
Yamlfy<OptimizationArgBase>::Yamlfy(const OptimizationArgBase *V) : 
  YamlfyTemplateBase(V) {

  }

void Yamlfy<OptimizationArgBase>::append(YAML::Emitter &Emitter, bool PrintReduced) {
  Emitter << YAML::BeginMap;

  Emitter << YAML::Key << "type";
  Emitter << YAML::Value << std::to_string(static_cast<int>(Value->Type));

  Emitter << YAML::Key << "value";
  Emitter << YAML::Value;
  switch(Value->Type) {
    case ValueType::Int:
      Yamlfy<int>(&(static_cast<OptimizationArg<int>*>(Value)->Value)).append(Emitter, false);
      break;
    case ValueType::Float:
      Yamlfy<double>(&(static_cast<OptimizationArg<double>*>(Value)->Value)).append(Emitter, false);
      break;
    case ValueType::Bool:
      Yamlfy<bool>(&(static_cast<OptimizationArg<bool>*>(Value)->Value)).append(Emitter, false);
      break;
    case ValueType::String:
      Yamlfy<std::string>(&(static_cast<OptimizationArg<std::string>*>(Value)->Value)).append(Emitter, false);
      break;
  }

  Emitter << YAML::EndMap;
}

void Yamlfy<OptimizationArgBase>::get(const YAML::Node &Node) {
  Value->Type = static_cast<ValueType>(Node["type"].as<int>());
  switch(Value->Type) {
    case ValueType::Int:
      static_cast<OptimizationArg<int>*>(Value)->Value = Node["value"].as<int>();
      break;
    case ValueType::Float:
      static_cast<OptimizationArg<double>*>(Value)->Value = Node["value"].as<double>();
      break;
    case ValueType::Bool:
      static_cast<OptimizationArg<bool>*>(Value)->Value = Node["value"].as<bool>();
      break;
    case ValueType::String:
      static_cast<OptimizationArg<std::string>*>(Value)->Value = Node["value"].as<std::string>();
      break;
  }
}

/*
 * ----------------------------------=
 * Struct: OptimizationArgBase
 */
OptimizationArgBase::OptimizationArgBase(ValueType Type) : Type(Type) {

}

OptimizationArgBase::~OptimizationArgBase() {

}

/*
 * ----------------------------------=
 * Struct: OptimizationProperties
 */
OptimizationProperties::OptimizationProperties() {

}

const OptimizationArgBase *OptimizationProperties::getOptimizationArg(uint64_t N) {
  assert(N < Args.size() && "Arg out of bounds.");
  return &Args[N]; 
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

ValueType OptimizationProperties::getArgType(uint64_t N) {
  assert(N < Args.size() && "Arg out of bounds.");
  return Args[N].Type;
}

uint64_t OptimizationProperties::getNumberOfArguments() {
  return Args.size();
}
