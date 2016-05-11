#include "gtest/gtest.h"

#include "pinhao/Support/JITExecutor.h"
#include "pinhao/PerformanceAnalyser/PAPIWrapper.h"

#include "ModuleReader.h"

using namespace pinhao;

std::string File = "../../benchmark/polybench-ll/2mm/2mm.bc";
std::shared_ptr<llvm::Module> Module;

TEST(PAPIWrapperTest, CountCycleTest) {
  long long Cycles = PAPIWrapper::getTotalCycles(*Module);
  ASSERT_NE(Cycles, 0);
  std::cout << "Total Cycles: " << Cycles << std::endl;
}

TEST(PAPIWrapperTest, CountInstructionsTest) {
  long long Instructions = PAPIWrapper::getTotalInstructions(*Module);
  ASSERT_NE(Instructions, 0);
  std::cout << "Total Instructions: " << Instructions << std::endl;
}

TEST(PAPIWrapperTest, CycleInstrTest) {
  PAPIWrapper::EventCodeVector Codes = { PAPI_TOT_CYC, PAPI_TOT_INS };
  PAPIWrapper::CounterVector Counters = PAPIWrapper::countEvents(*Module, Codes);

  std::cout << "{ ";
  for (auto C : Counters) {
    ASSERT_NE(C, 0);
    std::cout << C << " ";
  }
  std::cout << "}" << std::endl;
}

int main(int argc, char **argv) {
  initializeJITExecutor();

  ModuleReader Reader(File);
  Module = Reader.getModule();

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
