
#ifndef PINHAO_MODULE_READER_H
#define PINHAO_MODULE_READER_H

#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/IRReader/IRReader.h"   
#include "llvm/Support/SourceMgr.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/IR/LLVMContext.h"

class ModuleReader {
  private:
    std::shared_ptr<llvm::Module> M;

  public:
    ModuleReader(std::string &ModuleName) {
      llvm::LLVMContext &Context = llvm::getGlobalContext();
      llvm::SMDiagnostic Error;
      M = llvm::parseIRFile(ModuleName, Error, Context);
    }

    std::shared_ptr<llvm::Module> getModule() { return M; }
};

#endif
