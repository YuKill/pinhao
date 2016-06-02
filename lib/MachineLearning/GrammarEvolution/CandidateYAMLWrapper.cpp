/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Candidate.cpp
 */

#include "pinhao/MachineLearning/GrammarEvolution/Candidate.h"
#include "pinhao/MachineLearning/GrammarEvolution/GrammarEvolution.h"
#include "pinhao/MachineLearning/GrammarEvolution/Formula.h"

using namespace pinhao;

template<> void pinhao::YAMLWrapper::append(const Candidate &Cand, Emitter &E) {
  E << YAML::BeginMap;
  E << YAML::Key << "score" << YAML::Value << (double) Cand.Score;
  E << YAML::Key << "count" << YAML::Value << (int) Cand.Count;
  E << YAML::Key << "formulas" << YAML::Value;
  E << YAML::BeginSeq;
  for (auto &Pair : Cand) {
    E << YAML::BeginMap;
    E << YAML::Key << "name" << YAML::Value << Pair.first.Name;
    E << YAML::Key << "type" << YAML::Value << (int)Pair.first.Type;
    E << YAML::Key << "formula" << YAML::Value;
    auto &CandidateFB = *Pair.second;
    YAMLWrapper::append(CandidateFB, E);
    E << YAML::EndMap;
  }
  E << YAML::EndSeq;
  E << YAML::EndMap;
}

template<> void pinhao::YAMLWrapper::fill(Candidate &Cand, ConstNode &Node) {
  Cand.Score = Node["score"].as<double>();
  Cand.Count = Node["count"].as<int>();
  for (auto I = Node["formulas"].begin(), E = Node["formulas"].end(); I != E; ++I) {
    DecisionPoint DP((*I)["name"].as<std::string>(), (ValueType)(*I)["type"].as<int>());
    auto Form = YAMLWrapper::get<FormulaBase>((*I)["formula"]).release();
    Cand.insert(std::make_pair(DP, Form));
  }
}
