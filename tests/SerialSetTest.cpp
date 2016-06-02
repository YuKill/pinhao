#include "gtest/gtest.h"

#include "pinhao/Support/SerialSet.h"
#include "pinhao/Support/Random.h"

#include <limits>

using namespace pinhao;

TEST(SerialSetTest, IntSetTest) {
  int Size = 100;
  SerialSet<int> Set, ReadSet;
  for (int I = 0; I < Size; ++I) {
    int Rnd = UniformRandom::getRandomInt(0, std::numeric_limits<int>::max()-1);
    Set.insert(Rnd);
  }

  std::ostringstream Out;
  Set.print(Out);

  YAML::Node Node = YAML::Load(Out.str());
  YAMLWrapper::fill(ReadSet, Node);

  ASSERT_TRUE(Set == ReadSet);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
