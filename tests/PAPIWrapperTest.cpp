#include "gtest/gtest.h"

#include "pinhao/Support/JITExecutor.h"
#include "pinhao/PerformanceAnalyser/PAPIWrapper.h"

#include "ModuleReader.h"

using namespace pinhao;

std::string File = "../../benchmark/polybench-ll/2mm/2mm.bc";
std::shared_ptr<llvm::Module> Module;

TEST(PAPIWrapperTest, CountCycleTest) {
  auto PAPIRun = PAPIWrapper::getTotalCycles(*Module);
  ASSERT_EQ(PAPIRun.first, 0);
  ASSERT_NE(PAPIRun.second, 0);
  std::cout << "Total Cycles: " << PAPIRun.second << std::endl;
}

TEST(PAPIWrapperTest, CountInstructionsTest) {
  auto PAPIRun = PAPIWrapper::getTotalInstructions(*Module);
  ASSERT_EQ(PAPIRun.first, 0);
  ASSERT_NE(PAPIRun.second, 0);
  std::cout << "Total Instructions: " << PAPIRun.second << std::endl;
}

TEST(PAPIWrapperTest, CycleInstrTest) {
  PAPIWrapper::EventCodeVector Codes = { PAPI_TOT_CYC, PAPI_TOT_INS };
  auto PAPIRun = PAPIWrapper::countEvents(*Module, Codes);
  ASSERT_EQ(PAPIRun.first, 0);

  std::cout << "{ ";
  for (auto C : PAPIRun.second) {
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
