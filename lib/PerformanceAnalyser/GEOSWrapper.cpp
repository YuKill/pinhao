/*-------------------------- PINHAO project --------------------------*/

/**
 * @file GEOSWrapper.cpp
 */

#include "pinhao/PerformanceAnalyser/GEOSWrapper.h"
#include "pinhao/Support/YamlOptions.h"

#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/IRReader/IRReader.h"

#include "geos/GEOS.h"
#include "geos/CostEstimator/CostEstimatorOptions.h"
#include "geos/Profiling/GEOSProfiler.h"
#include "geos/Profiling/CallCostReader.h"

using namespace pinhao;

static config::YamlOpt<std::string> 
GEOSProfLibFile("geos-prof-lib", "The GEOSProfLib file name.", true, "");

static config::YamlOpt<std::string>
CallCostFile("call-cost", "File with call cost of extern functions.", true, "");

std::vector<double> GEOSWrapper::getAnalysisCost(std::shared_ptr<ProfileModule> PModule) {
  std::vector<double> Cost;
  CostEstimatorOptions Opts;

  Opts.AnalysisActivated = { RegisterUse };
  Cost.push_back(GEOS::analyseCost(PModule, Opts));

  /*  
   Opts.AnalysisActivated = { InstructionCache };
   */
  Cost.push_back(0.0);

  Opts.AnalysisActivated = { StaticInstruction };
  Cost.push_back(GEOS::analyseCost(PModule, Opts));

  Opts.AnalysisActivated = { TTIInstruction };
  Cost.push_back(GEOS::analyseCost(PModule, Opts));

  Opts.AnalysisActivated = { Branch };
  Cost.push_back(GEOS::analyseCost(PModule, Opts));

  Opts.AnalysisActivated = { Call };
  Cost.push_back(GEOS::analyseCost(PModule, Opts));

  Opts.AnalysisActivated = getAnalysisFor(NonArchSensitive);
  Cost.push_back(GEOS::analyseCost(PModule, Opts));

  Opts.AnalysisActivated = getAnalysisFor(ArchSensitive);
  Cost.push_back(GEOS::analyseCost(PModule, Opts));

  return Cost;
}

void GEOSWrapper::propagateToInstructions(ProfileModule &PModule) {
  for (auto &Function : *PModule.getLLVMModule()) {
    for (auto &BasicBlock : Function) {
      for (auto &Instr : BasicBlock) {

        PModule.setInstructionFrequency(Instr, 
            PModule.getBasicBlockFrequency(BasicBlock));

      }
    }
  }
}

void GEOSWrapper::loadCallCostFile(llvm::Module &M) {
  ProfileModule PModule(&M);
  loadCallCost(CallCostFile.get(), &PModule);
}

void GEOSWrapper::getFrequencies(std::shared_ptr<ProfileModule> PModule) {
  llvm::SMDiagnostic Error;
  llvm::LLVMContext &Context = llvm::getGlobalContext();
  llvm::Module *GEOSProfLib = parseIRFile(GEOSProfLibFile.get(), Error, Context).release();

  std::unique_ptr<GEOSProfiler> GProfiler(new GEOSProfiler());
  GProfiler->populateFrequency(PModule.get(), GEOSProfLib);
  propagateToInstructions(*PModule);
}

std::vector<double> GEOSWrapper::repairAndAnalyse(llvm::Module &M) {
  std::shared_ptr<ProfileModule> PModule(new ProfileModule(&M));
  PModule->repairProfiling();
  return getAnalysisCost(PModule);
}

std::vector<double> GEOSWrapper::getFrequenciesAndAnalyse(llvm::Module &M) {
  std::shared_ptr<ProfileModule> PModule(new ProfileModule(&M));
  getFrequencies(PModule);
  return getAnalysisCost(PModule);
}

