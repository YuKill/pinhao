#include "gtest/gtest.h"

#include "pinhao/Initialization.h"
#include "pinhao/Optimizer/OptimizationSet.h"
#include "pinhao/Support/YAMLWrapper.h"

#include <sstream>
#include <fstream>

#include "ModuleReader.h"

using namespace pinhao;

/*
 * This function generates the desired string from the specified
 * OptimizationSequence. It is one big function (without sub-function calls)
 * so that even though it's 'ugly', it is straight forward how it is
 * supposed to look like.
 *
 * - opt: adce
 * - opt: loop-unswitch
 *   args:
 *    - type: 2
 *      value: false
 *
 * Here, the cascade Yamlfy calls are tested.
 */
std::string getStringFrom(OptimizationSequence &Seq) {
  YAML::Emitter Emitter;

  Emitter << YAML::BeginSeq;

  for (auto &Info : Seq) {

    Emitter << YAML::BeginMap;
    Emitter << YAML::Key << "opt";
    Emitter << YAML::Value << Info.getName();

    if (Info.getNumberOfArguments() > 0) {

      Emitter << YAML::Key << "args";
      Emitter << YAML::Value << YAML::BeginSeq;

      for (uint64_t I = 0, E = Info.getNumberOfArguments(); I < E; ++I) {
        ValueType Type = Info.getArgType(I);

        Emitter << YAML::BeginMap;

        Emitter << YAML::Key << "type";
        Emitter << YAML::Value << std::to_string(static_cast<int>(Type));

        Emitter << YAML::Key << "value";
        Emitter << YAML::Value;
        switch(Type) {
          case ValueType::Int:
            Emitter << Info.getArg<int>(I);
            break;
          case ValueType::Float:
            Emitter << Info.getArg<double>(I);
            break;
          case ValueType::Bool:
            Emitter << Info.getArg<bool>(I);
            break;
          case ValueType::String:
            Emitter << Info.getArg<std::string>(I);
            break;
        }

        Emitter << YAML::EndMap;
      }

      Emitter << YAML::EndSeq;
    }

    Emitter << YAML::EndMap;
  }

  Emitter << YAML::EndSeq;

  return std::string(Emitter.c_str()) + "\n";
}

void checkOptimizationSequencePrint(OptimizationSequence &Sequence) {
  std::string Expected = getStringFrom(Sequence);
  std::ostringstream OSS;
  Sequence.print(OSS);
  ASSERT_EQ(OSS.str(), Expected);
}

TEST(OptimizerTest, PrintOptimization) {
  for (auto OptName : Optimizations) {
    OptimizationSet Set;
    Set.enableOptimization(getOptimization(OptName));
    Set.generateRandomSequenceIfNone(); 

    Set.DefaultSequence->print();
    checkOptimizationSequencePrint(*Set.DefaultSequence);
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

TEST(OptimizerTest, GenerateOptSetTest) {
  for (int I = 0; I < 10; ++I) {
    std::unique_ptr<OptimizationSequence> Sequence = OptimizationSequence::randomize(20); 
    std::unique_ptr<OptimizationSet> Set = OptimizationSet::generate(*Sequence.get());
    for (auto &Info : *Sequence) {
      ASSERT_TRUE(Set->hasEnabled(Info.getOptimization()));
      ASSERT_TRUE(Set->hasEnabled(Info));
    }
  }
}

TEST(OptimizerTest, EachOptimizationFunctionTest) {
  std::string Filepath("../../benchmark/polybench-ll/2mm/2mm.bc");
  ModuleReader Reader(Filepath);  
  llvm::Module *M = Reader.getModule().get();

  for (auto OptName : Optimizations) {
    if (getPassKind(getOptimization(OptName)) >= 4) continue;

    std::cout << "Applying " << OptName << "..." << std::endl;
    OptimizationSet Set;
    Set.enableOptimization(OptName);

    llvm::Module *NewModule = applyOptimizations(*M, M->getFunction("main"), &Set);
    ASSERT_NE(NewModule, nullptr);
  }
}

TEST(OptimizerTest, RandomizationFunctionTest) {
  std::string Filepath("../../benchmark/polybench-ll/2mm/2mm.bc");
  ModuleReader Reader(Filepath);  
  llvm::Module *M = Reader.getModule().get();

  for (int N = 1; N <= 10; ++N) {
    std::cout << "--------= Sequence: " << N << " =----------" << std::endl;
    OptimizationSet Set;
    Set.enableFunctionOptimizations();
    std::unique_ptr<OptimizationSequence> Seq(OptimizationSequence::generate(Set, N * 10)); 
    (void) applyOptimizations(*M, M->getFunction("main"), Seq.get());
    Seq->print();

    checkOptimizationSequencePrint(*Seq.get());
  }
}

TEST(OptimizerTest, YamlGetTest) {
  int Size = 100;
  const std::string Filename("sequence.yaml");
  for (uint64_t I = 0, E = 20; I < E; ++I) {
    std::ofstream OFS(Filename);
    std::unique_ptr<OptimizationSequence> Sequence = OptimizationSequence::randomize(Size); 
    Sequence->print(OFS);
    OFS.close();

    std::unique_ptr<OptimizationSequence> ReadSequence;
    YAML::Node Node = YAML::LoadFile(Filename);
    ReadSequence = YAMLWrapper::get<OptimizationSequence>(Node);
    
    std::less<OptimizationInfo> Less;
    for (uint64_t I = 0, E = Sequence->size(); I < E; ++I) {
      ASSERT_TRUE(!Less(Sequence->getOptimization(I), ReadSequence->getOptimization(I)) &&
          !Less(ReadSequence->getOptimization(I), Sequence->getOptimization(I))); 
    }
  }
}

int main(int argc, char **argv) {
  pinhao::initializeOptimizer();

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
