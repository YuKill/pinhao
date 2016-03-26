#include "gtest/gtest.h"

#include "pinhao/Features/Feature.h"
#include "pinhao/Features/FeatureRegistry.h"

#include "ModuleReader.h"

#include <fstream>

using namespace pinhao;

bool filesEqual(std::string Filename1, std::string Filename2) {
  std::ifstream If1(Filename1), If2(Filename2);
  std::string Str1, Str2;
  getline(If1, Str1, '\0');
  getline(If2, Str2, '\0');
  return Str1 == Str2;
}

TEST(YAMLTest, CFGModuleStaticFeatureTest) {
  const std::string YamlFile1("2mm-md-1.yaml");
  const std::string YamlFile2("2mm-md-2.yaml");
  std::ofstream Of1(YamlFile1), Of2(YamlFile2);
  std::string Benchmark("../../benchmark/polybench-ll/2mm/2mm.bc");
  ModuleReader Reader(Benchmark);
  std::unique_ptr<Feature> ModFeature = FeatureRegistry::get("cfg_md_static");
  ModFeature->processModule(*(Reader.getModule().get()));
  ModFeature->printYaml(Of1);

  ModFeature.reset(FeatureRegistry::get("cfg_md_static").release());
  YAML::Node LoadFile = YAML::LoadFile(YamlFile1);
  ModFeature->getFromYamlNode(LoadFile);
  ModFeature->printYaml(Of2);

  ASSERT_TRUE(filesEqual(YamlFile1, YamlFile2));
}

TEST(YAMLTest, CFGFunctionStaticFeatureTest) {
  const std::string YamlFile1("2mm-fn-1.yaml");
  const std::string YamlFile2("2mm-fn-2.yaml");
  std::ofstream Of1(YamlFile1), Of2(YamlFile2);
  std::string Benchmark("../../benchmark/polybench-ll/2mm/2mm.bc");
  ModuleReader Reader(Benchmark);
  std::unique_ptr<Feature> FnFeature = FeatureRegistry::get("cfg_fn_static");
  FnFeature->processModule(*(Reader.getModule().get()));
  FnFeature->printYaml(Of1);

  FnFeature.reset(FeatureRegistry::get("cfg_fn_static").release());
  YAML::Node LoadFile = YAML::LoadFile(YamlFile1);
  FnFeature->getFromYamlNode(LoadFile);
  FnFeature->printYaml(Of2);

  ASSERT_TRUE(filesEqual(YamlFile1, YamlFile2));
}

TEST(YAMLTest, CFGBasicBlockStaticFeatureTest) {
  const std::string YamlFile1("2mm-bb-1.yaml");
  const std::string YamlFile2("2mm-bb-2.yaml");
  std::ofstream Of1(YamlFile1), Of2(YamlFile2);
  std::string Benchmark("../../benchmark/polybench-ll/2mm/2mm.bc");
  ModuleReader Reader(Benchmark);
  std::unique_ptr<Feature> BBFeature = FeatureRegistry::get("cfg_bb_static");
  BBFeature->processModule(*(Reader.getModule().get()));
  BBFeature->printYaml(Of1);

  BBFeature.reset(FeatureRegistry::get("cfg_bb_static").release());
  YAML::Node LoadFile = YAML::LoadFile(YamlFile1);
  BBFeature->getFromYamlNode(LoadFile);
  BBFeature->printYaml(Of2);

  ASSERT_TRUE(filesEqual(YamlFile1, YamlFile2));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
