/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimizationSet.h
 */

#ifndef PINHAO_OPTIMIZATION_SET_H
#define PINHAO_OPTIMIZATION_SET_H

#include "pinhao/Optimizer/OptimizationSequence.h"

#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"

#include <string>
#include <vector>
#include <map>

namespace pinhao {
  class OptimizationSet;
  class OptimizationProperties;

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
    internalize,
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
    "internalize",
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

  void optimize(llvm::Module &Module, OptimizationSet &Set);
  void optimize(llvm::Function &Function, OptimizationSet &Set);

  /**
   * @brief Holds all enabled optimizations as well as the number of repetition
   * of each one.
   */
  class OptimizationSet {
    private:
      /// @brief A generated sequence used by the @a optimize method.
      OptimizationSequence *DefaultSequence;

      /// @brief The enabled optimizations paired with its properties.
      std::map<Optimization, OptimizationProperties> EnabledOptimizations;

    public:
      OptimizationSet();

      /// @brief Constructs an @a OptimizationSet from a given @a OptimizationSequence.
      static std::unique_ptr<OptimizationSet> getFromSequence(OptimizationSequence Seq);

      /// @brief Returns true if the @a Optimization @a Opt has already enabled.
      bool hasEnabled(Optimization Opt);

      /// @brief Removes @a Opt if enabled.
      void disableOptimization(Optimization Opt);
      /// @brief Enables the @a Opt with default properties.
      void enableOptimization(Optimization Opt);
      /// @brief Enables the @a Opt with some given property.
      void enableOptimization(Optimization Opt, OptimizationProperties Prop);
      /// @brief Enables the optimizations with default properties.
      void enableOptimizations(std::vector<Optimization> Opts);

      /// @brief Enables all optimizations with default properties.
      void enableAllOptimizations();
      /// @brief Enables all module optimizations with default properties.
      void enableModuleOptimizations();
      /// @brief Enables all function optimizations with default properties.
      void enableFunctionOptimizations();

      /// @brief Sets the properties of the optimization @a Opt.
      void setOptimizationProperties(Optimization Opt, OptimizationProperties Prop);
      /// @brief Gets a copy of the properties of the optimization @a Opt.
      OptimizationProperties getOptimizationProperties(Optimization Opt);

      /// @brief Sets the default sequence of optimization.
      void setDefaultSequence(OptimizationSequence Sequence);
      /// @brief Gets a copy of the default sequence of optimization.
      OptimizationSequence getDefaultSequence();

      /// @brief Returns the number of enabled optimizations.
      uint64_t size();

      typedef std::map<Optimization, OptimizationProperties>::iterator SetIterator;
      SetIterator begin() { return EnabledOptimizations.begin(); }
      SetIterator end() { return EnabledOptimizations.end(); }
  };

}

#endif


