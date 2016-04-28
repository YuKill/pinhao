/*-------------------------- PINHAO project --------------------------*/

/**
 * @file FeatureYAMLWrapper.h
 */

#ifndef PINHAO_FEATURE_YAML_WRAPPER_H
#define PINHAO_FEATURE_YAML_WRAPPER_H

#include "pinhao/Support/YAMLWrapper.h"
#include "pinhao/Support/YamlOptions.h"

#include "pinhao/Features/VectorFeature.h"
#include "pinhao/Features/MapFeature.h"
#include "pinhao/Features/MapVectorFeature.h"

using namespace pinhao;

static config::YamlOpt<bool> PrintReduced
("print-reduced", "Print all the informations in a reduced manner.", false, true);

static config::YamlOpt<bool> PrintComments
("print-comments", "Print the description, if defined.", false, false);

/*
 * -----------------------------------
 *  Overloading for: VectorFeature
 */
template <class Type> 
void pinhao::YAMLWrapper::fill(VectorFeature<Type> &Vf, ConstNode &Node) {
  YAML::Node Features = Node["features"];
  for (auto I = Features.begin(), E = Features.end(); I != E; ++I) {
    Type Elem;
    std::string FeatureName = I->first.as<std::string>();
    YAMLWrapper::fill<Type>(Elem, I->second);
    Vf.setValueOf(FeatureName, Elem);
  }
}

template <class Type> 
void pinhao::YAMLWrapper::append(const VectorFeature<Type> &Vf, Emitter &E) {
  E << YAML::BeginMap;
  E << YAML::Key << "feature-name" << YAML::Value << Vf.getName();
  E << YAML::Key << "features";
  E << YAML::Value << YAML::BeginMap;
  for (auto &InfoPair : Vf) {
    if (PrintReduced.get() && Vf.getValueOf(InfoPair.first) == 0) continue;

    E << YAML::Key << InfoPair.first << YAML::Value;
    YAMLWrapper::append<Type>(Vf.getValueOf(InfoPair.first), E);

    if (PrintComments.get()) 
      E << YAML::Comment(InfoPair.second);
  }
  E << YAML::EndMap;
  E << YAML::EndMap;
}

/*
 * -----------------------------------
 *  Overloading for: MapFeature<KType, VType>
 */
template <class KType, class VType> 
void YAMLWrapper::fill(MapFeature<KType, VType> &Mf, ConstNode &Node) {
  YAML::Node Values = Node["values"];
  for (auto I = Values.begin(), E = Values.end(); I != E; ++I) {
    KType Key;
    VType Value;

    YAMLWrapper::fill<KType>(Key, I->first);
    YAMLWrapper::fill<VType>(Value, I->second);
    Mf.setValueOfKey(Mf.getName(), Value, Key);
  }
}

template <class KType, class VType> 
void YAMLWrapper::append(const MapFeature<KType, VType> &Mf, Emitter &E) {
  E << YAML::BeginMap;
  E << YAML::Key << "feature-name" << YAML::Value << Mf.getName();

  if (PrintComments.get())
    E << YAML::Comment(Mf.getDescription());

  E << YAML::Key << "values";
  E << YAML::Value << YAML::BeginMap;
  MapFeature<KType, VType> &MfCast = const_cast<MapFeature<KType, VType>&>(Mf);
  for (auto &I = MfCast.beginKeys(), &Iend = MfCast.endKeys(); I != Iend; ++I) {
    E << YAML::Key;
    YAMLWrapper::append<KType>(*I, E);
    E << YAML::Value;
    YAMLWrapper::append<VType>(Mf.getValueOfKey(Mf.getName(), *I), E);
  }
  E << YAML::EndMap;
  E << YAML::EndMap;
}

/*
 * -----------------------------------
 *  Overloading for: MapVectorFeature
 */
template <class KType, class VType> 
void YAMLWrapper::fill(MapVectorFeature<KType, VType> &Mvf, ConstNode &Node) {
  YAML::Node Values = Node["values"];
  for (auto I = Values.begin(), E = Values.end(); I != E; ++I) {
    KType Key;
    YAMLWrapper::fill<KType>(Key, I->first);

    for (auto FI = I->second.begin(), FE = I->second.end(); FI != FE; ++FI) {
      VType Value;
      std::string FeatureName = FI->first.as<std::string>();
      YAMLWrapper::fill<VType>(Value, FI->second);
      Mvf.setValueOfKey(FeatureName, Value, Key);
    }

  }
}

template <class KType, class VType> 
void YAMLWrapper::append(const MapVectorFeature<KType, VType> &Mvf, Emitter &E) {
  E << YAML::BeginMap;
  E << YAML::Key << "feature-name" << YAML::Value << Mvf.getName();
  E << YAML::Key << "values";
  E << YAML::Value << YAML::BeginMap;
  MapVectorFeature<KType, VType> &MvfCast = const_cast<MapVectorFeature<KType, VType>&>(Mvf);
  for (auto &I = MvfCast.beginKeys(), &Iend = MvfCast.endKeys(); I != Iend; ++I) {
    E << YAML::Key;
    YAMLWrapper::append<KType>(*I, E);
    E << YAML::Value << YAML::BeginMap;

    for (auto &InfoPair : Mvf) {
      if (PrintReduced.get() && Mvf.getValueOfKey(InfoPair.first, *I) == 0) continue;
      E << YAML::Key << InfoPair.first << YAML::Value;
      YAMLWrapper::append<VType>(Mvf.getValueOfKey(InfoPair.first, *I), E);
      if (PrintComments.get())
        E << YAML::Comment(InfoPair.second);
    }

    E << YAML::EndMap;
  }
  E << YAML::EndMap;
  E << YAML::EndMap;
}

#endif
