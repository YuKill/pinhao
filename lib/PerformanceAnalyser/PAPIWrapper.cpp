/*-------------------------- PINHAO project --------------------------*/

/**
 * @file PAPIWrapper.cpp
 */

#include "pinhao/PerformanceAnalyser/PAPIWrapper.h"

#include <unistd.h>
#include <sys/wait.h>
#include <fstream>
#include <iostream>

using namespace pinhao;

void PAPIWrapper::initialize() {
  assert(PAPI_library_init(PAPI_VER_CURRENT) == PAPI_VER_CURRENT
      && "Error: PAPI library failed on initialization.");
}

bool PAPIWrapper::codesAreValid(EventCodeVector Codes) {
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

bool PAPIWrapper::addEvents(int EventSet, EventCodeVector Codes) {
  if (!codesAreValid(Codes)) return false;

  for (auto I : Codes)
    addEvent(EventSet, I);

  return true;
}

int PAPIWrapper::run(llvm::Module &Module, std::vector<std::string> Args, 
    char* const* Envp, EventCodeVector CodeVector, long long *Values) {
  int EventSet = 0;
  int ExitStatus = 0;
  std::string TmpName = ".papi-" + std::to_string(time(0));

  pid_t Pid = fork();

  if (Pid == 0) {
    std::cerr << "ChildPid: " << getpid() << std::endl;

    initialize();
    EventSet = createEventSet();
    if (!addEvents(EventSet, CodeVector))
      return 1;

    std::ofstream TmpOut(TmpName);

    JITExecutor JIT(Module);
    JIT.flushCache();

    assert(PAPI_start(EventSet) == PAPI_OK &&  
        "Error: PAPI library failed to start.");

    ExitStatus = JIT.run(Args, Envp);

    assert(PAPI_stop(EventSet, Values) == PAPI_OK &&  
        "Error: PAPI library failed to stop counters.");

    for (unsigned I = 0; I < CodeVector.size(); ++I) {
      TmpOut << Values[I] << std::endl;
      std::cout << "Values[" << I << "]: " << Values[I] << std::endl;
    }

    exit(ExitStatus);
  }
  
  waitpid(Pid, &ExitStatus, 0);
  if (ExitStatus != 0) {
    std::cerr << "Error while executing module." << std::endl; 
  } else {
    std::ifstream TmpIn(TmpName); 
    for (unsigned I = 0; I < CodeVector.size(); ++I)
      TmpIn >> Values[I];
  }

  return ExitStatus;

}

std::pair<int, PAPIWrapper::CounterVector> PAPIWrapper::countEvents(llvm::Module &Module,
    EventCodeVector Codes) {
  ArgVector Args = { "papi-profiling" };
  return countEvents(Module, Args, Codes);
}

std::pair<int, PAPIWrapper::CounterVector> PAPIWrapper::countEvents(llvm::Module &Module,
    ArgVector Args, EventCodeVector Codes) {
  CounterVector Counters;
  long long Values[Codes.size()];

  int ExitStatus = run(Module, Args, nullptr, Codes, Values);

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

  int ExitStatus = run(Module, Args, nullptr, { PAPI_TOT_CYC }, &Value);

  return std::make_pair(ExitStatus, Value);
}

std::pair<int, PAPIWrapper::LLong> PAPIWrapper::getTotalInstructions(llvm::Module &Module) {
  ArgVector Args = { "papi-profiling" };
  return getTotalInstructions(Module, Args);
}

std::pair<int, PAPIWrapper::LLong> PAPIWrapper::getTotalInstructions(llvm::Module &Module, 
    PAPIWrapper::ArgVector Args) {
  long long Value;

  int ExitStatus = run(Module, Args, nullptr, { PAPI_TOT_INS }, &Value);

  return std::make_pair(ExitStatus, Value);
}
