#include "gtest/gtest.h"

#include "pinhao/Support/KnowledgeBase.h"

#include <fstream>
#include <vector>

using namespace pinhao;

bool filesEqual(std::string Filename1, std::string Filename2) {
  std::ifstream If1(Filename1), If2(Filename2);
  std::string Str1, Str2;
  getline(If1, Str1, '\0');
  getline(If2, Str2, '\0');
  return Str1 == Str2;
}

TEST(KnowledgeBaseTest, IntBase) {
  std::string YamlFile1("kb-int-1.yaml");
  std::string YamlFile2("kb-int-2.yaml");
  KnowledgeBase<int> KB1;
  for (int I = 1; I < 60; ++I)
    KB1.insert(I * 10);
  KB1.exportData(YamlFile1);

  KnowledgeBase<int> KB2;
  KB2.importData(YamlFile1);
  KB2.exportData(YamlFile2);
  ASSERT_TRUE(filesEqual(YamlFile1, YamlFile2));
}

TEST(KnowledgeBaseTest, StringBase) {
  std::vector<std::string> Names = {
    "Aho", "Keith", "Dragon", "Tartara", "Turtle"
  };

  std::string YamlFile1("kb-string-1.yaml");
  std::string YamlFile2("kb-string-2.yaml");
  KnowledgeBase<std::string> KB1;
  for (auto S : Names) KB1.insert(S);
  KB1.exportData(YamlFile1);

  KnowledgeBase<std::string> KB2;
  KB2.importData(YamlFile1);
  KB2.exportData(YamlFile2);
  ASSERT_TRUE(filesEqual(YamlFile1, YamlFile2));
}

class IntWrapper {
  public:
    int x;
    IntWrapper(int x) : x(x) {}
    IntWrapper() : x(0) {}

    bool operator<(const IntWrapper &Rhs) const {
      return x < Rhs.x;
    }
};

namespace pinhao {
  template<> void YAMLWrapper::append<IntWrapper>(const IntWrapper &Value, Emitter &E);
  template<> void YAMLWrapper::fill<IntWrapper>(IntWrapper &Value, ConstNode &Node);
}

template<> void YAMLWrapper::append<IntWrapper>(const IntWrapper &Value, Emitter &E) {
  E << YAML::BeginMap;
  E << YAML::Key << "kind";
  E << YAML::Value << "intWrapper";
  E << YAML::Key << "value";
  E << YAML::Value << Value.x;
  E << YAML::EndMap;
}

template<> void YAMLWrapper::fill<IntWrapper>(IntWrapper &Value, ConstNode &Node) {
  Value.x = Node["value"].as<int>();
}

TEST(KnowledgeBaseTest, IntWrapperBase) {
  std::string YamlFile1("kb-intw-1.yaml");
  std::string YamlFile2("kb-intw-2.yaml");
  KnowledgeBase<IntWrapper> KB1;
  for (int I = 1; I < 40; ++I) 
    KB1.insert(IntWrapper(I * 100));
  KB1.exportData(YamlFile1);

  KnowledgeBase<IntWrapper> KB2;
  KB2.importData(YamlFile1);
  KB2.exportData(YamlFile2);
  ASSERT_TRUE(filesEqual(YamlFile1, YamlFile2));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
