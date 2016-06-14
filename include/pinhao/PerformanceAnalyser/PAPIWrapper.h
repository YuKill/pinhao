/*-------------------------- PINHAO project --------------------------*/

/**
 * @file PAPIWrapper.h
 */

#ifndef PINHAO_PAPI_WRAPPER_H
#define PINHAO_PAPI_WRAPPER_H

#include "pinhao/Support/JITExecutor.h"

#include <vector>
#include "papi.h"

namespace pinhao {

  /**
   * @brief This is a wrapper of the PAPI tool.
   */
  class PAPIWrapper {
    public:
      typedef long long LLong;
      typedef std::vector<int> EventCodeVector;
      typedef std::vector<LLong> CounterVector;
      typedef std::vector<std::string> ArgVector;

    private:
      /// @brief Initializes the papi library.
      static void initialize();
      /// @brief Creates an @a EventSet.
      static int createEventSet();
      /// @brief Returns true if all the events inside the vector are valid (available).
      static bool codesAreValid(EventCodeVector);
      /// @brief Adds an event to an @a EventSet.
      static void addEvent(int, int);
      /// @brief Returns true if it successfuly added all events to an @a EventSet.
      static bool addEvents(int, EventCodeVector);
      /// @brief Runs the @a llvm::Module, while counting the events.
      static int run(llvm::Module&, ArgVector, char* const*, EventCodeVector, long long*);

    public:
      /// @brief Counts specific events detailed by the user.
      static std::pair<int, CounterVector> countEvents(llvm::Module&, EventCodeVector);
      /// @brief Returns the total number of clocks.
      static std::pair<int, LLong> getTotalCycles(llvm::Module&);
      /// @brief Returns the total number of instructions completed.
      static std::pair<int, LLong> getTotalInstructions(llvm::Module&);
  
      /// @brief Counts specific events detailed by the user. For execution that
      /// requires arguments.
      static std::pair<int, CounterVector> countEvents(llvm::Module&, ArgVector, EventCodeVector);
      /// @brief Returns the total number of clocks. For execution that requires
      /// arguments.
      static std::pair<int, LLong> getTotalCycles(llvm::Module&, ArgVector);
      /// @brief Returns the total number of instructions completed. For execution
      /// that requires arguments.
      static std::pair<int, LLong> getTotalInstructions(llvm::Module&, ArgVector);
  };

}

#endif


