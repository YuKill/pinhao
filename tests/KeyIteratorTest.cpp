#include "gtest/gtest.h"

#include "pinhao/Features/Features.h"
#include "pinhao/Features/FeatureRegistry.h"

#include "pinhao/Support/Iterator.h"

#include "ModuleReader.h"

using namespace pinhao;

std::shared_ptr<llvm::Module> Module;

TEST(KeyIteratorTest, MapFeaturesFunctionTest) {
  std::unique_ptr<Feature> DNAFeature = FeatureRegistry::get("function-dna");
  ASSERT_NE(DNAFeature.get(), nullptr);

  DNAFeature->processModule(*Module);

  auto &I = beginKeys<std::string, std::string>(DNAFeature.get());
  auto &E = endKeys<std::string, std::string>(DNAFeature.get());

  std::map<std::string, bool> FunctionMap;
  for (; I != E; ++I) {
    std::cout << "Found " << *I << std::endl;
    FunctionMap[*I] = true;
  }

  for (auto &F : *Module) {
    if (F.getBasicBlockList().size() == 0) continue;
    std::cout << "Checking " << F.getName().str() << std::endl;
    ASSERT_TRUE(FunctionMap[F.getName().str()]);
  }
}

TEST(KeyIteratorTest, MapFeaturesTest) {
  std::unique_ptr<Feature> DNAFeature = FeatureRegistry::get("function-dna");
  ASSERT_NE(DNAFeature.get(), nullptr);

  DNAFeature->processModule(*Module);

  MappedFeature<std::string, std::string> *Mapped = 
    static_cast<MappedFeature<std::string, std::string>*>(DNAFeature.get());

  std::map<std::string, bool> FunctionMap;
  for (auto &I = Mapped->beginKeys(), &E = Mapped->endKeys(); I != E; ++I) {
    std::cout << "Found " << *I << std::endl;
    FunctionMap[*I] = true;
  }

  for (auto &F : *Module) {
    if (F.getBasicBlockList().size() == 0) continue;
    std::cout << "Checking " << F.getName().str() << std::endl;
    ASSERT_TRUE(FunctionMap[F.getName().str()]);
  }
}

TEST(KeyIteratorTest, MapVectorFeaturesTest) {
  std::unique_ptr<Feature> BasicBlockFeatures = FeatureRegistry::get("cfg_bb_static");
  ASSERT_NE(BasicBlockFeatures.get(), nullptr);

  BasicBlockFeatures->processModule(*Module);

  MappedFeature<void*, uint64_t> *Mapped = 
    static_cast<MappedFeature<void*, uint64_t>*>(BasicBlockFeatures.get());

  std::map<void*, bool> BBMap;
  for (auto &I = Mapped->beginKeys(), &E = Mapped->endKeys(); I != E; ++I)
    BBMap[*I] = true;

  for (auto &F : *Module)
    for (auto &BB : F)
      ASSERT_TRUE(BBMap[&BB]);
}

TEST(KeyIteratorTest, MapVectorFeaturesFunctionTest) {
  std::unique_ptr<Feature> BasicBlockFeatures = FeatureRegistry::get("cfg_bb_static");
  ASSERT_NE(BasicBlockFeatures.get(), nullptr);

  BasicBlockFeatures->processModule(*Module);

  auto &I = beginKeys<void*, uint64_t>(BasicBlockFeatures.get());
  auto &E = endKeys<void*, uint64_t>(BasicBlockFeatures.get());

  std::map<void*, bool> BBMap;
  for (; I != E; ++I)
    BBMap[*I] = true;

  for (auto &F : *Module)
    for (auto &BB : F)
      ASSERT_TRUE(BBMap[&BB]);
}

int main(int argc, char **argv) {
  std::string Filename("../../benchmark/polybench-ll/2mm/2mm.bc");
  ModuleReader Reader(Filename);
  Module = Reader.getModule();

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
