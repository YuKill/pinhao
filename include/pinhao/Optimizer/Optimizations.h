/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Optimizations.h
 */

#ifndef PINHAO_OPTIMIZATIONS_H
#define PINHAO_OPTIMIZATIONS_H

#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"

#include <string>
#include <vector>
#include <map>

namespace pinhao {
  class OptimizationSet;
  class OptimizationSequence;

  void initializeOptimizer();

  /**
   * @brief Enumerator of the available optimizations.
   */
  enum class Optimization {
    adce = 0,
    alwaysInline,
    argpromotion,
    bbVectorize,
    breakCritEdges,
    codegenprepare,
    constmerge,
    constprop,
    dce,
    deadargelim,
    die,
    dse,
    functionattrs,
    globaldce,
    globalopt,
    gvn,
    indvars,
    inlineOpt,
    instcombine,
    // internalize, <- Probably some llvm bug.
    ipconstprop,
    ipsccp,
    jumpThreading,
    lcssa,
    licm,
    loopDeletion,
    loopExtract,
    loopExtractSingle,
    loopReduce,
    loopRotate,
    loopSimplify,
    loopUnroll,
    loopUnswitch,
    loweratomic,
    lowerinvoke,
    lowerswitch,
    mem2reg,
    memcpyopt,
    mergefunc,
    mergereturn,
    partialInliner,
    pruneEh,
    reassociate,
    reg2mem,
    scalarrepl,
    sccp,
    simplifycfg,
    sink,
    strip,
    stripDeadDebugInfo,
    stripDeadPrototypes,
    stripDebugDeclare,
    stripNondebug,
    tailcallelim
  };

  /**
   * Vector with the name of the optimizations.
   */
  static const std::vector<std::string> Optimizations = {
    "adce",
    "always-inline",
    "argpromotion",
    "bb-vectorize",
    "break-crit-edges",
    "codegenprepare",
    "constmerge",
    "constprop",
    "dce",
    "deadargelim",
    "die",
    "dse",
    "functionattrs",
    "globaldce",
    "globalopt",
    "gvn",
    "indvars",
    "inline",
    "instcombine",
    // "internalize",
    "ipconstprop",
    "ipsccp",
    "jump-threading",
    "lcssa",
    "licm",
    "loop-deletion",
    "loop-extract",
    "loop-extract-single",
    "loop-reduce",
    "loop-rotate",
    "loop-simplify",
    "loop-unroll",
    "loop-unswitch",
    "loweratomic",
    "lowerinvoke",
    "lowerswitch",
    "mem2reg",
    "memcpyopt",
    "mergefunc",
    "mergereturn",
    "partial-inliner",
    "prune-eh",
    "reassociate",
    "reg2mem",
    "scalarrepl",
    "sccp",
    "simplifycfg",
    "sink",
    "strip",
    "strip-dead-debug-info",
    "strip-dead-prototypes",
    "strip-debug-declare",
    "strip-nondebug",
    "tailcallelim"
  };

  /// @brief Gets a @a Optimization corresponding to the @a OptName.
  Optimization getOptimization(std::string OptName);
  /// @brief Gets the optimization name corresponding to the @a Opt.
  std::string getOptimizationName(Optimization Opt);

  /// @brief Gets the @a PassKind of the @a Opt.
  int getPassKind(Optimization Opt);

  /** 
   * @brief Applies a generated @a OptimizationSequence to a module, from the @a OptimizationSet.
   * @return The optimized module.
   */
  llvm::Module *applyOptimizations(llvm::Module &Module, OptimizationSet *Set);
  /** 
   * @brief Applies the @a OptimizationSequence to a module.
   * @return The optimized module.
   */
  llvm::Module *applyOptimizations(llvm::Module &Module, OptimizationSequence *Seq);
  /** 
   * @brief Applies a generated @a OptimizationSequence to a function which has name @a FunctionName, 
   * from the @a OptimizationSet.
   *
   * @return The optimized module.
   */
  llvm::Module *applyOptimizations(llvm::Module &Module, std::string FunctionName, OptimizationSet *Set);
  /** 
   * @brief Applies the @a OptimizationSequence to a function whose name is @a FunctionName.
   * @return The optimized module.
   */
  llvm::Module *applyOptimizations(llvm::Module &Module, std::string FunctionName, OptimizationSequence *Seq);
  /** 
   * @brief Applies a generated @a OptimizationSequence to the function @a Function, from the @a OptimizationSet.
   * @return The optimized module.
   */
  llvm::Module *applyOptimizations(llvm::Module &Module, llvm::Function *Function, OptimizationSet *Set);
  /**
   * @brief Applies the @a OptimizationSequence to a function.
   * @return The optimized function's module.
   */
  llvm::Module *applyOptimizations(llvm::Module &Module, llvm::Function *Function, OptimizationSequence *Seq);

}

#endif


