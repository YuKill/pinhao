/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Main.cpp
 */

#include "pinhao/MachineLearning/GrammarEvolution/SimpleGrammarEvolution.h"

#include "pinhao/PinhaoOptions.h"
#include "pinhao/InitializationRoutines.h"
#include "pinhao/Features/FeatureSet.h"

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"

using namespace pinhao;

static config::YamlOpt<std::string> KnowledgeBaseName
("kb-name", "The name of the knowledge base.", true, "");

static config::YamlOpt<std::string> KnowledgeBasePath
("kb-path", "The path of the knowledge base.", false, "./");

static config::YamlOpt<int> BestCandidatesNumber
("best-n", "The number of candidates chosen by elitism.", false, 5);

static config::YamlOpt<double> EvolveProbability
("evol-prob", "The probability that one candidate evolves.", false, 0.2);

static config::YamlOpt<double> MaxEvolutionRate
("max-evol", "The maximum rate of evolution for each candidate.", false, 0.3);

static config::YamlOpt<double> MutateProbability
("mut-prob", "The probability that when evolving, it occurs a mutation.", false, 0.3);

static config::YamlOpt<int> GenerationsNumber
("generations", "The maximum number of generations.", false, 5);

llvm::Module *readModule() {
  std::string FilePath = LLVMModulePath.get() + "/" + LLVMModuleName.get();
  std::cout << "Reading module at: " << FilePath << std::endl;
  llvm::SMDiagnostic Err;
  llvm::LLVMContext &Context = llvm::getGlobalContext();
  return llvm::parseIRFile(FilePath, Err, Context).release();
}

void initialize() {
  initializeJITExecutor(); 
  initializeOptimizer(); 
  initializeCFGModuleStaticFeatures(); 
}

int main(int argc, char **argv) {
  parseCommandLine(argc, argv);
  initialize();

  std::shared_ptr<llvm::Module> Module(readModule());

  FeatureSet::enable("cfg_md_static");
  std::shared_ptr<FeatureSet> Set = FeatureSet::get();
  auto SetPass = new FeatureSetWrapperPass(&Set);
  SetPass->runOnModule(*Module);

  std::string KnowledgeBaseFP = KnowledgeBasePath.get() + KnowledgeBaseName.get();
  SimpleGrammarEvolution SGE(Module, KnowledgeBaseFP, EvolveProbability.get(), 
      MaxEvolutionRate.get(), MutateProbability.get());

  SGE.run(BestCandidatesNumber.get(), GenerationsNumber.get(), Set);
}
