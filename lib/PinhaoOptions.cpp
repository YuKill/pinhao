/*-------------------------- PINHAO project --------------------------*/

/**
 * @file PinhaoOptions.cpp
 */

#include "pinhao/PinhaoOptions.h"

using namespace pinhao;

llvm::cl::opt<std::string> ConfigFilename
(llvm::cl::Positional, llvm::cl::desc("<config file>"), llvm::cl::Required);

void pinhao::parseCommandLine(int Argc, char **Argv) {
  llvm::cl::ParseCommandLineOptions(Argc, Argv);
  config::parseOptions(ConfigFilename);
}
