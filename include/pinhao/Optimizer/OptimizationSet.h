/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimizationSet.h
 */

#ifndef PINHAO_OPTIMIZATION_SET_H
#define PINHAO_OPTIMIZATION_SET_H

#include "pinhao/Optimizer/Optimizations.h"
#include "pinhao/Optimizer/OptimizationInfo.h"
#include "pinhao/Optimizer/OptimizationSequence.h"
#include "pinhao/Support/Types.h"

#include <string>
#include <vector>
#include <map>

namespace std {

  template<> struct less<pinhao::OptimizationInfo> {

    template <class Type>
      bool isArgLess(const pinhao::OptimizationInfo &Lhs, const pinhao::OptimizationInfo &Rhs, uint64_t I) const {
        if (Lhs.getArg<Type>(I) != Rhs.getArg<Type>(I))
          return Lhs.getArg<Type>(I) < Rhs.getArg<Type>(I); 
        if (I < Lhs.getNumberOfArguments() - 1)
          return checkArg(Lhs, Rhs, I+1);
        return false;
      }

    bool checkArg(const pinhao::OptimizationInfo &Lhs, const pinhao::OptimizationInfo &Rhs, uint64_t I) const {
      assert(Lhs.getArgType(I) == Rhs.getArgType(I) && "Argument type of same optimizations doesn't match.");
      switch (Lhs.getArgType(I)) {
        case pinhao::ValueType::Int:    return isArgLess<int>(Lhs, Rhs, I);
        case pinhao::ValueType::Float:  return isArgLess<double>(Lhs, Rhs, I);
        case pinhao::ValueType::String: return isArgLess<std::string>(Lhs, Rhs, I);
        case pinhao::ValueType::Bool:   return isArgLess<bool>(Lhs, Rhs, I);
      }
      return false;
    }

    bool operator()(const pinhao::OptimizationInfo &Lhs, const pinhao::OptimizationInfo &Rhs) const {
      if (Lhs.getOptimization() != Rhs.getOptimization())
        return Lhs.getOptimization() < Rhs.getOptimization();

      assert(Lhs.getNumberOfArguments() == Rhs.getNumberOfArguments() && 
          "Number of arguments of same optimizations doesn't match.");

      if (Lhs.getNumberOfArguments() > 0)
        return checkArg(Lhs, Rhs, 0);
      return false;
    } 

  };

}

namespace pinhao {

  /**
   * @brief Holds all enabled optimizations as well as the number of repetition
   * of each one.
   */
  class OptimizationSet {
    private:
      /// @brief The enabled optimizations paired with its repetition number.
      std::map<OptimizationInfo, uint64_t> EnabledOptimizations;

    public:
      OptimizationSet();
      ~OptimizationSet();

      /// @brief The default sequence in which it will be compiled.
      OptimizationSequence *DefaultSequence;

      /// @brief Constructs an @a OptimizationSet from a given @a OptimizationSequence.
      static std::unique_ptr<OptimizationSet> generate(OptimizationSequence Seq);

      /// @brief Returns true if the @a Optimization @a Opt has already been enabled.
      bool hasEnabled(Optimization Opt);
      /// @brief Returns true if the @a Optimization @a Opt with the same information
      /// has already been enabled.
      bool hasEnabled(OptimizationInfo Info);

      /// @brief Removes @a Opt, if enabled.
      void disableOptimization(Optimization Opt);
      /// @brief Removes @a Opt with information @a Info, if enabled.
      void disableOptimization(OptimizationInfo Info);
      /// @brief Enables the @a Opt with default properties.
      void enableOptimization(Optimization Opt, uint64_t Repetition = 1);
      /// @brief Enables a optimization with specific @a OptimizationInfo.
      void enableOptimization(OptimizationInfo Info, uint64_t Repetition = 1);
      /// @brief Enables the optimizations with default properties.
      void enableOptimizations(std::vector<Optimization> Opts);

      /// @brief Enables all optimizations with default properties.
      void enableAllOptimizations();
      /// @brief Enables all module optimizations with default properties.
      void enableModuleOptimizations();
      /// @brief Enables all function optimizations with default properties.
      void enableFunctionOptimizations();

      /**
       * @brief Enables if doesn't exist, add one to the repetition if exists.
       * @details
       * If the optimization with default information is already enabled, the number 
       * of repetition is increased by one. Otherwise, it enables the optimization.
       */
      void addOptimization(Optimization Opt);
      /**
       * @brief Enables if doesn't exist, add one to the repetition if exists.
       * @details
       * If there is already an optimization with the same information as @a Info,
       * the number of repetition is increased by one. Otherwise, it enables the
       * optimization.
       */
      void addOptimization(OptimizationInfo Info);

      /// @brief Sets the repetition number to @a Rep, if the optimization is enabled.
      void setRepetition(Optimization Opt, uint64_t Rep);
      /// @brief Sets the repetition number to @a Rep, if the optimization with information
      /// @a Info is enabled.
      void setRepetition(OptimizationInfo Info, uint64_t Rep);
      /// @brief Gets the repetition number to @a Rep, if the optimization is enabled.
      uint64_t getRepetition(Optimization Opt);
      /// @brief Gets the repetition number to @a Rep, if the optimization with information
      /// @a Info is enabled.
      uint64_t getRepetition(OptimizationInfo Info);

      /// @brief Generates a random sequence (it will be the default sequence),
      /// in order to compile.
      void generateRandomSequenceIfNone();

      /// @brief Returns the number of enabled optimizations.
      uint64_t size();

      typedef std::map<OptimizationInfo, uint64_t>::iterator SetIterator;
      SetIterator begin() { return EnabledOptimizations.begin(); }
      SetIterator end() { return EnabledOptimizations.end(); }
  };

}

#endif


