#include "gtest/gtest.h"

#include "pinhao/Support/YamlOptions.h"

#include "yaml-cpp/yaml.h"

#include <fstream>

using namespace pinhao;

config::YamlOpt<std::string> Name("name", "The name of the person.", true, "");
config::YamlOpt<std::string> Surname("surname", "The surname of the person.", true, "");
config::YamlOpt<int> Age("age", "The age of the person.", true, 0);

void setupNode(YAML::Node &Node) {
  Node["name"] = "Alfred Vaino";
  Node["surname"] = "Aho";
  Node["age"] = 74;
}

TEST(YamlOptionsTest, GlobalVarTest) {
  YAML::Node Node;
  setupNode(Node);

  ASSERT_EQ(Name.get(), "");
  ASSERT_EQ(Surname.get(), "");
  ASSERT_EQ(Age.get(), 0);

  config::parseOptions(Node);

  ASSERT_EQ(Name.get(), "Alfred Vaino");
  ASSERT_EQ(Surname.get(), "Aho");
  ASSERT_EQ(Age.get(), 74);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
