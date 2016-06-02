/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Candidate.cpp
 */

#include "pinhao/MachineLearning/GrammarEvolution/Candidate.h"
#include "pinhao/MachineLearning/GrammarEvolution/GrammarEvolution.h"
#include "pinhao/MachineLearning/GrammarEvolution/Formula.h"
#include "pinhao/MachineLearning/GrammarEvolution/Evolution.h"

#include "pinhao/Optimizer/OptimizationSet.h"

using namespace pinhao;

bool pinhao::CompareByScore::operator()(const Candidate &Lhs, const Candidate &Rhs) {
  if (Lhs.Score != Rhs.Score)
    return Lhs.Score > Rhs.Score; 
  return Lhs > Rhs;
}

/*
 * --------------------------------------
 *  Class: Candidate
 */

pinhao::Candidate::~Candidate() {

}

void pinhao::Candidate::evolve(double E, Evolution *GEvo) {
  std::vector<int> Processed(size(), 0); 
  int Quantity = E * size();
  while (Quantity) {
    int Hi = UniformRandom::getRandomInt(0, size()-1); 
    if (Processed[Hi]) continue;

    auto &Form = this->get(Hi);
    Form->evolve(GEvo);
    simplifyFormula(Form);

    --Quantity;
  }
}

void pinhao::Candidate::generateMissing(std::vector<DecisionPoint> DPVector, FeatureSet *Set) {
  if (DPVector.size() == size()) return;
  for (auto DP : DPVector) {
    if (count(DP) > 0) continue; 
    auto Form = generateFormula(Set, DP.Type).release();
    simplifyFormula(Form);
    insert(std::make_pair(DP, Form));
  }
}

Candidate *pinhao::Candidate::clone() const {
  Candidate *Clone = new Candidate();
  Clone->Score = Score;
  Clone->Count = Count;
  for (auto &Pair : *this)
    Clone->insert(std::make_pair(Pair.first, Pair.second->clone().release()));
  return Clone;
}

FormulaBase* &pinhao::Candidate::get(uint64_t N) {
  assert(N < size() && "Candidate heuristic out of bounds.");
  auto Pair = begin();
  while (N--) ++Pair;
  return Pair->second;
}

bool pinhao::Candidate::operator<(const Candidate &Rhs) const {
  if (size() != Rhs.size()) return size() < Rhs.size();
  for (auto &Pair : *this) {
    auto &ThisFB = *Pair.second;
    auto &RhsFB = *Rhs.at(Pair.first);
    if (ThisFB != RhsFB)
      return ThisFB < RhsFB;
  }
  return false;
}

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
