#include "gtest/gtest.h"

#include "pinhao/Support/YamlOptions.h"

#include "yaml-cpp/yaml.h"

#include <fstream>

using namespace pinhao;

const std::string Filename("dummy.yaml");

/*
 * string-opt: string
 * int-opt: 42
 * vector-opt:
 *   - 10
 *   - 20
 *   - 30
 *  map-opt:
 *   ten: 10
 *   twenty: 20
 *   thirty: 30
 */

void createDummyFile() {
  std::ofstream Out(Filename);
  YAML::Emitter Emitter(Out);

  Emitter << YAML::BeginMap;
  Emitter << YAML::Key << "string-opt" << YAML::Value << "string";
  Emitter << YAML::Key << "int-opt" << YAML::Value << "42";

  Emitter << YAML::Key << "vector-opt" << YAML::BeginSeq;
  Emitter << "10" << "20" << "30" << YAML::EndSeq;

  Emitter << YAML::Key << "map-opt" << YAML::BeginMap;
  Emitter << YAML::Key << "ten" << YAML::Value << "10";
  Emitter << YAML::Key << "twenty" << YAML::Value << "20";
  Emitter << YAML::Key << "thirty" << YAML::Value << "30";
  Emitter << YAML::EndMap;

  Emitter << YAML::EndMap;
}

void checkOpt(config::YamlOpt<std::string> &Opt, YAML::Node Result) {
  ASSERT_EQ(Opt.get(), Result.as<std::string>());
}

void checkOpt(config::YamlOpt<int> &Opt, YAML::Node Result) {
  ASSERT_EQ(Opt.get(), Result.as<int>());
}

void checkOpt(config::YamlOpt<std::vector<int>> &Opt, YAML::Node Result) {
  std::vector<int> Vector = Opt.get();
  ASSERT_EQ(Vector.size(), 3);
  for (int I = 0, E = Vector.size(); I < E; ++I)
    ASSERT_EQ(Vector[I], Result[I].as<int>());
}

void checkOpt(config::YamlOpt<std::map<std::string, int>> &Opt, YAML::Node Result) {
  std::map<std::string, int> Map = Opt.get();
  ASSERT_EQ(Map.size(), 3);
  for (auto I = Result.begin(), E = Result.end(); I != E; ++I) {
    std::string Key = I->first.as<std::string>();
    ASSERT_TRUE(Map.count(Key) > 0);
    ASSERT_EQ(Map[Key], I->second.as<int>());
  }
}

TEST(YamlOptionsTest, StringTest) {
  YAML::Node Node;
  Node["string-opt"] = "string";
  config::YamlOpt<std::string> String("string-opt", "The String Option.", true, "");
  ASSERT_EQ(String.get(), "");

  config::parseOptions(Node);
  checkOpt(String, Node["string-opt"]);
}

TEST(YamlOptionsTest, IntTest) {
  YAML::Node Node;
  Node["int-opt"] = "50";
  config::YamlOpt<int> Int("int-opt", "The Int Option.", true, 10);
  ASSERT_EQ(Int.get(), 10);

  config::parseOptions(Node);
  checkOpt(Int, Node["int-opt"]);
}

TEST(YamlOptionsTest, VectorTest) {
  YAML::Node Node;
  Node["vector-opt"].push_back(10);
  Node["vector-opt"].push_back(20);
  Node["vector-opt"].push_back(30);

  config::YamlOpt<std::vector<int>> VectorInt("vector-opt", "The Vector Option.", true);
  ASSERT_EQ(VectorInt.get().size(), 0);

  config::parseOptions(Node);
  checkOpt(VectorInt, Node["vector-opt"]);
}

TEST(YamlOptionsTest, MapTest) {
  YAML::Node Node;
  Node["map-opt"]["ten"] = "10";
  Node["map-opt"]["twenty"] = "20";
  Node["map-opt"]["thirty"] = "30";
  config::YamlOpt<std::map<std::string, int>> MapInt("map-opt", "The Map Option.", true);
  ASSERT_EQ(MapInt.get().size(), 0);

  config::parseOptions(Node);
  checkOpt(MapInt, Node["map-opt"]);
}

TEST(YamlOptionsTest, ParseTest) {
  config::YamlOpt<std::string> String("string-opt", "The String Option.", true, "Default");
  config::YamlOpt<int> Int("int-opt", "The Int Option.", true, 10);
  config::YamlOpt<std::vector<int>> Vector("vector-opt", "The Vector Option.", true);
  config::YamlOpt<std::map<std::string, int>> Map("map-opt", "The Map Option.", true);

  ASSERT_EQ(String.get(), "Default");
  ASSERT_EQ(Int.get(), 10);
  ASSERT_EQ(Vector.get().size(), 0);
  ASSERT_EQ(Map.get().size(), 0);

  config::parseOptions(Filename);
  YAML::Node Node = YAML::LoadFile(Filename);

  checkOpt(String, Node["string-opt"]);
  checkOpt(Int, Node["int-opt"]);
  checkOpt(Vector, Node["vector-opt"]);
  checkOpt(Map, Node["map-opt"]);
}

int main(int argc, char **argv) {
  createDummyFile();

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
