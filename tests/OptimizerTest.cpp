#include "gtest/gtest.h"

#include "pinhao/Optimizer/OptimizationSet.h"

#include "ModuleReader.h"

using namespace pinhao;


TEST(OptimizerTest, PrintOptimization) {
  for (auto OptName : Optimizations) {
    OptimizationSet Set;
    Set.enableOptimization(getOptimization(OptName));
    Set.generateRandomSequenceIfNone(); 
    Set.DefaultSequence->print();
  }
}

TEST(OptimizerTest, EachOptimizationTest) {
  std::string Filepath("../../benchmark/polybench-ll/2mm/2mm.bc");
  ModuleReader Reader(Filepath);  
  llvm::Module *M = Reader.getModule().get();

  for (auto OptName : Optimizations) {
    std::cout << "Applying " << OptName << "..." << std::endl;
    OptimizationSet Set;
    Set.enableOptimization(getOptimization(OptName));

    llvm::Module *NewModule = applyOptimizations(*M, &Set);
    ASSERT_NE(NewModule, nullptr);
  }
}

TEST(OptimizerTest, RandomizationTest) {
  std::string Filepath("../../benchmark/polybench-ll/2mm/2mm.bc");
  ModuleReader Reader(Filepath);  
  llvm::Module *M = Reader.getModule().get();

  for (int N = 0; N < 10; ++N) {
    std::cout << "--------= Sequence: " << N << " =----------" << std::endl;
    std::unique_ptr<OptimizationSequence> Seq(OptimizationSequence::randomize(10)); 
    (void) applyOptimizations(*M, Seq.get());
    Seq->print();
  }
}

int main(int argc, char **argv) {
  pinhao::initializeOptimizer();

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
