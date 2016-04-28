/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimzerYAML.cpp
 */

#include "pinhao/Support/YAMLWrapper.h"
#include "pinhao/Support/YamlOptions.h"

#include "pinhao/Optimizer/OptimizationInfo.h"
#include "pinhao/Optimizer/OptimizationSequence.h"

using namespace pinhao;

static config::YamlOpt<bool> PrintReduced
("print-reduced", "Print all the informations in a reduced manner.", false, true);

static config::YamlOpt<bool> PrintComments
("print-comments", "Print the description, if defined.", false, false);

/*
 * -----------------------------------
 *  Overloading for: OptimizationArgBase
 */
template<> void pinhao::YAMLWrapper::fill<OptimizationArgBase>(OptimizationArgBase &OptArg, ConstNode &Node) {
  OptArg.Type = static_cast<ValueType>(Node["type"].as<int>());
  switch(OptArg.Type) {
    case ValueType::Int:
      static_cast<OptimizationArg<int>&>(OptArg).Value = Node["value"].as<int>();
      break;
    case ValueType::Float:
      static_cast<OptimizationArg<double>&>(OptArg).Value = Node["value"].as<double>();
      break;
    case ValueType::Bool:
      static_cast<OptimizationArg<bool>&>(OptArg).Value = Node["value"].as<bool>();
      break;
    case ValueType::String:
      static_cast<OptimizationArg<std::string>&>(OptArg).Value = Node["value"].as<std::string>();
      break;
  }
}

template<> void pinhao::YAMLWrapper::append<OptimizationArgBase>(const OptimizationArgBase &OptArg, Emitter &E) {
  E << YAML::BeginMap;

  E << YAML::Key << "type";
  E << YAML::Value << std::to_string(static_cast<int>(OptArg.Type));
  E << YAML::Key << "value";
  E << YAML::Value;
  switch(OptArg.Type) {
    case ValueType::Int:
      YAMLWrapper::append(static_cast<const OptimizationArg<int>&>(OptArg).Value, E);
      break;
    case ValueType::Float:
      YAMLWrapper::append(static_cast<const OptimizationArg<double>&>(OptArg).Value, E);
      break;
    case ValueType::Bool:
      YAMLWrapper::append(static_cast<const OptimizationArg<bool>&>(OptArg).Value, E);
      break;
    case ValueType::String:
      YAMLWrapper::append(static_cast<const OptimizationArg<std::string>&>(OptArg).Value, E);
      break;
  }
  E << YAML::EndMap;
}

/*
 * -----------------------------------
 *  Overloading for: OptimizationInfo
 */
template<> void pinhao::YAMLWrapper::fill<OptimizationInfo>(OptimizationInfo &Info, ConstNode &Node) {
  Info = OptimizationInfo(getOptimization(Node["opt"].as<std::string>()));

  if (Info.getNumberOfArguments() > 0 && Node["args"]) 
    for (uint64_t I = 0, E = Info.getNumberOfArguments(); I < E; ++I) {
      OptimizationArgBase *ArgI = const_cast<OptimizationArgBase*>(Info.getOptimizationArg(I));
      YAMLWrapper::fill(*ArgI, Node["args"][I]);
    }
}

template<> void pinhao::YAMLWrapper::append<OptimizationInfo>(const OptimizationInfo &Info, Emitter &E) {
  E << YAML::BeginMap;
  E << YAML::Key << "opt";
  E << YAML::Value << Info.getName();
  if (Info.getNumberOfArguments() > 0) {
    E << YAML::Key << "args";
    E << YAML::Value << YAML::BeginSeq;
    for (uint64_t I = 0, Iend = Info.getNumberOfArguments(); I < Iend; ++I) 
      YAMLWrapper::append(*Info.getOptimizationArg(I), E);
    E << YAML::EndSeq;
  }
  E << YAML::EndMap;
}

/*
 * -----------------------------------
 *  Overloading for: OptimizationSequence
 */
template<> void pinhao::YAMLWrapper::fill<OptimizationSequence>(OptimizationSequence &OSeq, ConstNode &Node) {
  for (auto I = Node.begin(), E = Node.end(); I != E; ++I) {
    OptimizationInfo Info;
    YAMLWrapper::fill(Info, *I);
    OSeq.push_back(Info);
  }
}

template<> void pinhao::YAMLWrapper::append<OptimizationSequence>(const OptimizationSequence &OSeq, Emitter &E) {
  E << YAML::BeginSeq;
  for (auto OptInfo : OSeq) 
    YAMLWrapper::append(OptInfo, E);
  E << YAML::EndSeq;
}
