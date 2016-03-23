/*-------------------------- PINHAO project --------------------------*/

/**
 * @file PAPIMultFeatures.cpp
 * @brief This file implements the features extracted by the PAPI tool by
 * multiplexing.
 */

#include "pinhao/Features/Features.h"
#include "pinhao/Support/JITExecutor.h"

#include "papi.h"

using namespace pinhao;

namespace {

  /**
   * @brief This class collects dynamic information with PAPI by multiplexing it.
   */
  class PAPIMultFeatures : public VectorFeature<uint64_t> {
    private:
      std::vector<std::string> Args;

      int getAvailableEvents(int *EventCodes);
      void getMeasures(long long *Values, int *EventCodes, int Size);

    public:
      ~PAPIMultFeatures() {}
      PAPIMultFeatures(FeatureInfo *Info) : VectorFeature<uint64_t>(Info) {
        Args.push_back("ProgramName");
      } 

      std::unique_ptr<Feature> clone() const override;

      void processModule(llvm::Module &Module) override;
  };

}

void PAPIMultFeatures::processModule(llvm::Module &Module) {
  CompositeFeatureInfo *CInfo = static_cast<CompositeFeatureInfo*>(this->Info.get());
  int Size, *EventCodes = new int[CInfo->getNumberOfSubFeatures()], 
      EventSet = PAPI_NULL;
  long long *Values;

  assert(PAPI_library_init(PAPI_VER_CURRENT) == PAPI_VER_CURRENT
      && "Error: PAPI library failed on initialization.");

  assert(PAPI_multiplex_init() == PAPI_OK &&  
      "Error: This architecture doens't support hardware \
      counters multiplexing!");

  assert(PAPI_create_eventset(&EventSet) == PAPI_OK &&  
      "Error: PAPI library failed to create an event set."); 
  assert(PAPI_assign_eventset_component(EventSet, 0) == PAPI_OK &&  
      "Error: PAPI library cound't bind component.");

  assert(PAPI_set_multiplex(EventSet) == PAPI_OK &&  
      "Error: PAPI library failed to set multiplex to the Event Set."); 

  JITExecutor JIT(Module);
  Size   = getAvailableEvents(EventCodes);
  Values = new long long[Size]();

  assert(PAPI_add_events(EventSet, EventCodes, Size) == PAPI_OK &&
      "Error: PAPI library failed to add events.");

  JIT.flushCache();
  assert(PAPI_start(EventSet) == PAPI_OK &&  
      "Error: PAPI library failed to start.");

  JIT.run(Args, nullptr);

  assert(PAPI_stop(EventSet, Values) == PAPI_OK &&  
      "Error: PAPI library failed to stop counters.");

  getMeasures(Values, EventCodes, Size);
}

int PAPIMultFeatures::getAvailableEvents(int *EventCodes) {
  int Size = 0;
  const std::string Prefix("PAPI_");

  for (auto &Pair : *this) {
    int Code = 0;
    char EvName[PAPI_MAX_STR_LEN];

    strcpy(EvName, Prefix.c_str());
    strcat(EvName, Pair.first.c_str());

    if (PAPI_event_name_to_code(EvName, &Code) == PAPI_OK &&
        PAPI_query_event(Code) == PAPI_OK)
      EventCodes[Size++] = Code;
  }

  return Size;
}

void PAPIMultFeatures::getMeasures(long long *Values, int *EventCodes, int Size) {
  const std::string Prefix("PAPI_");
  for (int I = 0; I < Size; ++I) {
    std::string EvName;
    char Buff[PAPI_MAX_STR_LEN];

    if (PAPI_event_code_to_name(EventCodes[I], Buff) != PAPI_OK) continue;

    EvName = Buff;
    setValueOf(EvName.substr(Prefix.length(), std::string::npos), Values[I]);
  }
}

std::unique_ptr<Feature> PAPIMultFeatures::clone() const {
  PAPIMultFeatures *Clone = new PAPIMultFeatures(*this);
  return std::unique_ptr<Feature>(Clone);
}

static std::map<std::string, std::string> SubFeatures = {
  { "CA_SNP", "Requests for a snoop" },
  { "CA_SHR", "Requests for exclusive access to shared cache line" },
  { "CA_CLN", "Requests for exclusive access to clean cache line" },
  { "CA_INV", "Requests for cache line invalidation" },
  { "CA_ITV", "Requests for cache line intervention" },
  { "L1_ICM", "Level 1 instruction cache misses" },
  { "L1_TCM", "Level 1 cache misses" },
  { "L1_LDM", "Level 1 load misses" },
  { "L1_STM", "Level 1 store misses" },
  { "L1_DCM", "Level 1 data cache misses" },
  { "L1_DCH", "Level 1 data cache hits" },
  { "L1_DCA", "Level 1 data cache accesses" },
  { "L1_DCR", "Level 1 data cache reads" },
  { "L1_DCW", "Level 1 data cache writes" },
  { "L1_ICH", "Level 1 instruction cache hits" },
  { "L1_ICA", "Level 1 instruction cache accesses" },
  { "L1_ICR", "Level 1 instruction cache reads" },
  { "L1_ICW", "Level 1 instruction cache writes" },
  { "L1_TCH", "Level 1 total cache hits" },
  { "L1_TCA", "Level 1 total cache accesses" },
  { "L1_TCR", "Level 1 total cache reads" },
  { "L1_TCW", "Level 1 total cache writes" },
  { "L2_DCM", "Level 2 data cache misses" },
  { "L2_ICM", "Level 2 instruction cache misses" },
  { "L2_TCM", "Level 2 cache misses" },
  { "L2_LDM", "Level 2 load misses" },
  { "L2_STM", "Level 2 store misses" },
  { "L2_DCH", "Level 2 data cache hits" },
  { "L2_DCA", "Level 2 data cache accesses" },
  { "L2_DCR", "Level 2 data cache reads" },
  { "L2_DCW", "Level 2 data cache writes" },
  { "L2_ICH", "Level 2 instruction cache hits" },
  { "L2_ICA", "Level 2 instruction cache accesses" },
  { "L2_ICR", "Level 2 instruction cache reads" },
  { "L2_ICW", "Level 2 instruction cache writes" },
  { "L2_TCH", "Level 2 total cache hits" },
  { "L2_TCA", "Level 2 total cache accesses" },
  { "L2_TCR", "Level 2 total cache reads" },
  { "L2_TCW", "Level 2 total cache writes" },
  { "L3_DCM", "Level 3 data cache misses" },
  { "L3_ICM", "Level 3 instruction cache misses" },
  { "L3_TCM", "Level 3 cache misses" },
  { "L3_LDM", "Level 3 load misses" },
  { "L3_STM", "Level 3 store misses" },
  { "L3_DCH", "Level 3 data cache hits" },
  { "L3_DCA", "Level 3 data cache accesses" },
  { "L3_DCR", "Level 3 data cache reads" },
  { "L3_DCW", "Level 3 data cache writes" },
  { "L3_ICH", "Level 3 instruction cache hits" },
  { "L3_ICA", "Level 3 instruction cache accesses" },
  { "L3_ICR", "Level 3 instruction cache reads" },
  { "L3_ICW", "Level 3 instruction cache writes" },
  { "L3_TCH", "Level 3 total cache hits" },
  { "L3_TCA", "Level 3 total cache accesses" },
  { "L3_TCR", "Level 3 total cache reads" },
  { "L3_TCW", "Level 3 total cache writes" },
  { "BRU_IDL", "Cycles branch units are idle" },
  { "FXU_IDL", "Cycles integer units are idle" },
  { "FPU_IDL", "Cycles floating point units are idle" },
  { "LSU_IDL", "Cycles load/store units are idle" },
  { "TLB_DM", "Data translation lookaside buffer misses" },
  { "TLB_IM", "Instruction translation lookaside buffer misses" },
  { "TLB_TL", "Total translation lookaside buffer misses" },
  { "BTAC_M", "Branch target address cache misses" },
  { "PRF_DM", "Data prefetch cache misses" },
  { "TLB_SD", "Translation lookaside buffer shootdowns" },
  { "CSR_FAL", "Failed store conditional instructions" },
  { "CSR_SUC", "Successful store conditional instructions" },
  { "CSR_TOT", "Total store conditional instructions" },
  { "MEM_SCY", "Cycles Stalled Waiting for memory accesses" },
  { "MEM_RCY", "Cycles Stalled Waiting for memory Reads" },
  { "MEM_WCY", "Cycles Stalled Waiting for memory writes" },
  { "STL_ICY", "Cycles with no instruction issue" },
  { "FUL_ICY", "Cycles with maximum instruction issue" },
  { "STL_CCY", "Cycles with no instructions completed" },
  { "FUL_CCY", "Cycles with maximum instructions completed" },
  { "HW_INT", "Hardware interrupts" },
  { "BR_UCN", "Unconditional branch instructions" },
  { "BR_CN", "Conditional branch instructions" },
  { "BR_TKN", "Conditional branch instructions taken" },
  { "BR_NTK", "Conditional branch instructions not taken" },
  { "BR_MSP", "Conditional branch instructions mispredicted" },
  { "BR_PRC", "Conditional branch instructions correctly predicted" },
  { "FMA_INS", "FMA instructions completed" },
  { "TOT_IIS", "Instructions issued" },
  { "TOT_INS", "Instructions completed" },
  { "INT_INS", "Integer instructions" },
  { "FP_INS", "Floating point instructions" },
  { "LD_INS", "Load instructions" },
  { "SR_INS", "Store instructions" },
  { "BR_INS", "Branch instructions" },
  { "VEC_INS", "Vector/SIMD instructions (could include integer)" },
  { "RES_STL", "Cycles stalled on any resource" },
  { "FP_STAL", "Cycles the FP unit(s) are stalled" },
  { "TOT_CYC", "Total cycles" },
  { "LST_INS", "Load/store instructions completed" },
  { "SYC_INS", "Synchronization instructions completed" },
  { "FAD_INS", "Floating point add instructions" },
  { "FML_INS", "Floating point multiply instructions" },
  { "FDV_INS", "Floating point divide instructions" },
  { "FSQ_INS", "Floating point square root instructions" },
  { "FNV_INS", "Floating point inverse instructions" },
  { "FP_OPS", "Floating point operations" },
  { "SP_OPS", "Floating point operations; optimized to count scaled single precision vector operations" },
  { "DP_OPS", "Floating point operations; optimized to count scaled double precision vector operations" },
  { "VEC_SP", "Single precision vector/SIMD instructions" },
  { "VEC_DP", "Double precision vector/SIMD instructions" },
  { "REF_CYC", "Reference clock cycles" }
};

static RegisterFeature<PAPIMultFeatures> 
X(new CompositeFeatureInfo("papi-feat-m", "Dynamic information collected by the PAPI tool.", 
      FeatureInfo::IntType, FeatureInfo::Dynamic, SubFeatures));
