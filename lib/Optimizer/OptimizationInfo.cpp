/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimizationInfo.cpp
 */

#include "pinhao/Optimizer/Optimizations.h"
#include "pinhao/Optimizer/OptimizationSet.h"
#include "pinhao/Optimizer/OptimizationSequence.h"

#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Vectorize.h"
#include "llvm/Transforms/IPO.h"

using namespace pinhao;

/*
 * ----------------------------------=
 * Class: Yamlfy<OptimizationInfo>
 */
Yamlfy<OptimizationInfo>::Yamlfy(const OptimizationInfo *V) : 
  YamlfyTemplateBase(V) {

  }

void Yamlfy<OptimizationInfo>::append(YAML::Emitter &Emitter, bool PrintReduced) {
  Emitter << YAML::BeginMap;

  Emitter << YAML::Key << "opt";
  Emitter << YAML::Value << Value->getName();

  if (Value->getNumberOfArguments() > 0) {
    Emitter << YAML::Key << "args";
    Emitter << YAML::Value << YAML::BeginSeq;
    for (uint64_t I = 0, E = Value->getNumberOfArguments(); I < E; ++I) 
      Yamlfy<OptimizationArgBase>(Value->getOptimizationArg(I)).append(Emitter, false);
    Emitter << YAML::EndSeq;
  }

  Emitter << YAML::EndMap;
}

void Yamlfy<OptimizationInfo>::get(const YAML::Node &Node) {
  *Value = OptimizationInfo(getOptimization(Node["opt"].as<std::string>()));

  if (Value->getNumberOfArguments() > 0 && Node["args"]) 
    for (uint64_t I = 0, E = Value->getNumberOfArguments(); I < E; ++I) 
      Yamlfy<OptimizationArgBase>(Value->getOptimizationArg(I)).get(Node["args"][I]);
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
 * Class: OptimizationInfo
 */
void OptimizationInfo::copy(const OptimizationInfo &Info) {
  Opt = Info.Opt;
  for (uint64_t I = 0, E = Info.getNumberOfArguments(); I < E; ++I) {
    switch (Info.getArgType(I)) {
      case ValueType::Int:
        Args.push_back(new OptimizationArg<int>(ValueType::Int, Info.getArg<int>(I)));
        break;

      case ValueType::Bool:
        Args.push_back(new OptimizationArg<bool>(ValueType::Bool, Info.getArg<bool>(I)));
        break;

      case ValueType::Float:
        Args.push_back(new OptimizationArg<double>(ValueType::Float, Info.getArg<double>(I)));
        break;

      case ValueType::String:
        Args.push_back(new OptimizationArg<std::string>(ValueType::String, Info.getArg<std::string>(I)));
        break;
    }
  }
}

OptimizationInfo::~OptimizationInfo() {
  for (auto Arg : Args)
    delete Arg; 
}

OptimizationInfo::OptimizationInfo() {

}

OptimizationInfo::OptimizationInfo(std::string OptName) :
  OptimizationInfo(pinhao::getOptimization(OptName)) {

  }

OptimizationInfo::OptimizationInfo(Optimization Opt) : Opt(Opt) {
  switch (Opt) {
    case Optimization::gvn:
      Args.push_back(new OptimizationArg<bool>(ValueType::Bool, true));
      break;
    case Optimization::jumpThreading:
      Args.push_back(new OptimizationArg<int>(ValueType::Int, -1));
      break;

    case Optimization::simplifycfg:
      Args.push_back(new OptimizationArg<int>(ValueType::Int, -1));
      break;

    case Optimization::loopRotate:
      Args.push_back(new OptimizationArg<int>(ValueType::Int, -1));
      break;

    case Optimization::loopUnswitch:
      Args.push_back(new OptimizationArg<bool>(ValueType::Bool, false));
      break;

    case Optimization::loopUnroll:
      Args = std::vector<OptimizationArgBase*> {
        new OptimizationArg<int>(ValueType::Int, -1),
            new OptimizationArg<int>(ValueType::Int, -1),
            new OptimizationArg<int>(ValueType::Int, -1),
            new OptimizationArg<int>(ValueType::Int, -1)
      };
      break;

    case Optimization::scalarrepl:
      Args = std::vector<OptimizationArgBase*> {
        new OptimizationArg<int>(ValueType::Int, -1),
            new OptimizationArg<bool>(ValueType::Bool, true),
            new OptimizationArg<int>(ValueType::Int, -1),
            new OptimizationArg<int>(ValueType::Int, -1),
            new OptimizationArg<int>(ValueType::Int, -1)
      };
      break;

    default:
      break;
  };
}

OptimizationInfo::OptimizationInfo(const OptimizationInfo &Info) {
  copy(Info);
}

OptimizationInfo &OptimizationInfo::operator=(const OptimizationInfo &Info) {
  copy(Info);
  return *this;
}

const OptimizationArgBase *OptimizationInfo::getOptimizationArg(uint64_t N) const {
  assert(N < Args.size() && "Arg out of bounds.");
  return Args[N];
}

std::string OptimizationInfo::getName() const {
  return getOptimizationName(Opt);
}

Optimization OptimizationInfo::getOptimization() const {
  return Opt;
}

llvm::Pass *OptimizationInfo::createPass() {
  switch (Opt) {
    case Optimization::gvn:
      return llvm::createGVNPass(getArg<bool>(0));

    case Optimization::jumpThreading:
      return llvm::createJumpThreadingPass(getArg<int>(0));

    case Optimization::loopRotate:
      return llvm::createLoopRotatePass(getArg<int>(0));

    case Optimization::loopUnroll:
      return llvm::createLoopUnrollPass(getArg<int>(0), getArg<int>(1), getArg<int>(2), getArg<int>(3));

    case Optimization::loopUnswitch:
      return llvm::createLoopUnswitchPass(getArg<bool>(0));

    case Optimization::scalarrepl:
      return llvm::createScalarReplAggregatesPass(
          getArg<int>(0), 
          getArg<bool>(1), 
          getArg<int>(2), 
          getArg<int>(3), 
          getArg<int>(4));

    case Optimization::simplifycfg:
      return llvm::createCFGSimplificationPass(getArg<int>(0));

    default:
      break;
  };  

  llvm::PassRegistry *Registry = llvm::PassRegistry::getPassRegistry();
  const llvm::PassInfo *PI = Registry->getPassInfo(getName());
  return PI->createPass();
}

ValueType OptimizationInfo::getArgType(uint64_t N) const {
  assert(N < Args.size() && "Arg out of bounds.");
  return Args[N]->Type;
}

uint64_t OptimizationInfo::getNumberOfArguments() const {
  return Args.size();
}
