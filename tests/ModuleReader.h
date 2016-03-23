
#ifndef PINHAO_MODULE_READER_H
#define PINHAO_MODULE_READER_H

#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/IRReader/IRReader.h"   
#include "llvm/Support/SourceMgr.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/IR/LLVMContext.h"

using namespace llvm;

class ModuleReader {
  private:
    std::shared_ptr<Module> M;

  public:
    ModuleReader(std::string &ModuleName) {
      LLVMContext &Context = getGlobalContext();
      SMDiagnostic Error;
      M = parseIRFile(ModuleName, Error, Context);
    }

    std::shared_ptr<Module> getModule() { return M; }
};

#endif
