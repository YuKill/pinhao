/*-------------------------- PINHAO project --------------------------*/

/**
 * @file PinhaoOptions.h
 */

#ifndef PINHAO_OPTIONS_H
#define PINHAO_OPTIONS_H

#include "pinhao/Support/YamlOptions.h"

#include "llvm/Support/CommandLine.h"

extern llvm::cl::opt<std::string> ConfigFilename;

namespace pinhao {

  static config::YamlOpt<std::string> LLVMModuleName
    ("module", "The desired module name.", true, "");

  static config::YamlOpt<std::string> LLVMModulePath
    ("module-path", "The desired module path (without its name).", false, "./");

  void parseCommandLine(int, char**);

}

#endif
