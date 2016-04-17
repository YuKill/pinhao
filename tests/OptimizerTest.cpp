#include "gtest/gtest.h"

#include "pinhao/Optimizer/OptimizationSet.h"

#include "ModuleReader.h"

using namespace llvm;

TEST(OptimizerTest, EachOptimizationTest) {
  std::string Filepath("../../benchmark/polybench-ll/2mm/2mm.bc");
  ModuleReader Reader(Filepath);  
  Module *M = Reader.getModule().get();

  for (auto OptName : pinhao::Optimizations) {
    std::cout << "Applying " << OptName << "..." << std::endl;
    pinhao::OptimizationSet Set;
    Set.enableOptimization(pinhao::getOptimization(OptName));

    Module *NewModule = pinhao::applyOptimizations(*M, &Set);
    ASSERT_NE(NewModule, nullptr);
  }
}

int main(int argc, char **argv) {
  pinhao::initializeOptimizer();

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
