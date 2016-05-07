#include "gtest/gtest.h"

#include "pinhao/Support/Random.h"

#include <iostream>

using namespace pinhao;

TEST(RandomTest, RandomIntTest) {
  int Start = 0;
  int End = 100;
  for (int I = 0; I < 10000; ++I) {
    int RandomNumber = UniformRandom::getRandomInt(Start, End); 
    ASSERT_LE(RandomNumber, End);
    ASSERT_GE(RandomNumber, Start);
  }
}

TEST(RandomTest, RandomRealTest) {
  double Last = 0.0;
  double Diff = 0.000001;
  for (int I = 0; I < 10000; ++I) {
    double RandomNumber = UniformRandom::getRandomReal(); 
    std::cout << "Real: " << RandomNumber << std::endl;
    ASSERT_FALSE(RandomNumber <= Last + Diff && RandomNumber >= Last - Diff);
    Last = RandomNumber;
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
