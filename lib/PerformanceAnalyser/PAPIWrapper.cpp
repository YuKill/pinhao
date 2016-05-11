/*-------------------------- PINHAO project --------------------------*/

/**
 * @file PAPIWrapper.cpp
 */

#include "pinhao/PerformanceAnalyser/PAPIWrapper.h"

using namespace pinhao;

void PAPIWrapper::init() {
  assert(PAPI_library_init(PAPI_VER_CURRENT) == PAPI_VER_CURRENT
      && "Error: PAPI library failed on initialization.");
}

bool PAPIWrapper::codesAreValid(PAPIWrapper::EventCodeVector Codes) {
  for (auto I : Codes)
    if (PAPI_query_event(I) != PAPI_OK)
      return false;
  return true;
}

int PAPIWrapper::createEventSet() {
  int EventSet = PAPI_NULL;

  assert(PAPI_create_eventset(&EventSet) == PAPI_OK &&
      "Error: PAPI library failed to create an event set.");
  assert(PAPI_assign_eventset_component(EventSet, 0) == PAPI_OK &&
      "Error: PAPI library cound't bind component.");

  return EventSet;
}

void PAPIWrapper::addEvent(int EventSet, int Code) {
  assert(PAPI_add_event(EventSet, Code) == PAPI_OK &&
     "Error: PAPI library failed to add event.");
}

bool PAPIWrapper::addEvents(int EventSet, PAPIWrapper::EventCodeVector Codes) {
  if (!codesAreValid(Codes)) return false;

  for (auto I : Codes)
    addEvent(EventSet, I);

  return true;
}

int PAPIWrapper::run(llvm::Module &Module, std::vector<std::string> Args, 
    char* const* Envp, int EventSet, long long *Values) {
  int ExitStatus = 0;

  JITExecutor JIT(Module);
  JIT.flushCache();

  assert(PAPI_start(EventSet) == PAPI_OK &&  
      "Error: PAPI library failed to start.");

  ExitStatus = JIT.run(Args, Envp);

  assert(PAPI_stop(EventSet, Values) == PAPI_OK &&  
      "Error: PAPI library failed to stop counters.");

  return ExitStatus;
}

std::pair<int, PAPIWrapper::CounterVector> PAPIWrapper::countEvents(llvm::Module &Module,
    PAPIWrapper::EventCodeVector Codes) {
  ArgVector Args = { "papi-profiling" };
  return countEvents(Module, Args, Codes);
}

std::pair<int, PAPIWrapper::CounterVector> PAPIWrapper::countEvents(llvm::Module &Module,
    PAPIWrapper::ArgVector Args, PAPIWrapper::EventCodeVector Codes) {
  CounterVector Counters;
  long long Values[Codes.size()];

  init(); 
  int EventSet = createEventSet();

  if (!addEvents(EventSet, Codes))
    return std::make_pair(1, Counters);

  int ExitStatus = run(Module, Args, nullptr, EventSet, Values);

  for (uint64_t I = 0, E = Codes.size(); I < E; ++I)
    Counters.push_back(Values[I]);

  return std::make_pair(ExitStatus, Counters);
}

std::pair<int, PAPIWrapper::LLong> PAPIWrapper::getTotalCycles(llvm::Module &Module) {
  ArgVector Args = { "papi-profiling" };
  return getTotalCycles(Module, Args);
}

std::pair<int, PAPIWrapper::LLong> PAPIWrapper::getTotalCycles(llvm::Module &Module, 
    PAPIWrapper::ArgVector Args) {
  long long Value;

  init();
  int EventSet = createEventSet();
  addEvent(EventSet, PAPI_TOT_CYC);

  int ExitStatus = run(Module, Args, nullptr, EventSet, &Value);

  return std::make_pair(ExitStatus, Value);
}

std::pair<int, PAPIWrapper::LLong> PAPIWrapper::getTotalInstructions(llvm::Module &Module) {
  ArgVector Args = { "papi-profiling" };
  return getTotalInstructions(Module, Args);
}

std::pair<int, PAPIWrapper::LLong> PAPIWrapper::getTotalInstructions(llvm::Module &Module, 
    PAPIWrapper::ArgVector Args) {
  long long Value;

  init();
  int EventSet = createEventSet();
  addEvent(EventSet, PAPI_TOT_INS);

  int ExitStatus = run(Module, Args, nullptr, EventSet, &Value);

  return std::make_pair(ExitStatus, Value);
}
