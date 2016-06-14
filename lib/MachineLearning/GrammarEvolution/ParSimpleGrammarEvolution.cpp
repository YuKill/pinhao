/*-------------------------- PINHAO project --------------------------*/

/**
 * @file ParSimpleGrammarEvolution.cpp
 */

#include "pinhao/MachineLearning/GrammarEvolution/ParSimpleGrammarEvolution.h"
#include "pinhao/MachineLearning/GrammarEvolution/Candidate.h"
#include "pinhao/MachineLearning/GrammarEvolution/Formula.h"

#include "pinhao/Optimizer/OptimizationSet.h"
#include "pinhao/PerformanceAnalyser/PAPIWrapper.h"

using namespace pinhao;

/*
 * -------------------------------------
 *  Class: ParSimpleGrammarEvolution
 */
void pinhao::ParSimpleGrammarEvolution::addPreDefinedDecisionPoints() {
  for (auto OName : Optimizations) {
    OptimizationInfo Info(getOptimization(OName)); 
    DecisionPoint DP(OName, ValueType::Bool); 
    this->DecisionPoints.push_back(DP);

    for (unsigned I = 0; I < Info.getNumberOfArguments(); ++I) {
      DecisionPoint DP("~" + OName + "." + std::to_string(I), Info.getArgType(I)); 
      DecisionPoints.push_back(DP);
    }
  }
}

llvm::Module *pinhao::ParSimpleGrammarEvolution::
compileWithCandidate(llvm::Module *Module, Candidate &C, FeatureSet *Set) {

  OptimizationSet OptSet;
  OptimizationSequence OptSequence;

  for (auto &Pair : C) {
    DecisionPoint DP(Pair.first);
    FormulaBase *Form = Pair.second;
    Form->solveFor(Set);
    if (DP.Name[0] != '~') {

      std::string OptName = Pair.first.Name;

      bool EnableOpt = getFormulaValue<bool>(Form);
      if (EnableOpt)
        OptSet.addOptimization(getOptimization(OptName));

    } else {

      size_t End = DP.Name.find_first_of(".", 1); 
      std::string OName = DP.Name.substr(1, End - 1);
      if (OptSet.hasEnabled(getOptimization(OName))) {
        auto &OptInfo = OptSet.getOptimizationInfo(getOptimization(OName));
        uint64_t N = std::stoull(DP.Name.substr(End+1));
        switch (OptInfo.getArgType(N)) {

          case ValueType::Int:
            OptInfo.setArg<int>(N, getFormulaValue<int>(Form));
            break;
          case ValueType::Bool:
            OptInfo.setArg<bool>(N, getFormulaValue<bool>(Form));
            break;
          case ValueType::Float:
            OptInfo.setArg<double>(N, getFormulaValue<double>(Form));
            break;

          default:
            break;

        }
      }
    }
  }

  for (auto O : Sequence) {
    Optimization Opt = (Optimization) O;
    if (!OptSet.hasEnabled(Opt)) continue;

    OptimizationInfo Info = OptSet.getOptimizationInfo(Opt); 
    OptSequence.push_back(Info);
  }

  return applyOptimizations(*Module, &OptSet);
}

