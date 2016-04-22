/*-------------------------- PINHAO project --------------------------*/

/**
 * @file InitializeRoutines.h
 */

#ifndef PINHAO_INITIALIZE_ROUTINES_H
#define PINHAO_INITIALIZE_ROUTINES_H

namespace pinhao {

  void initializeGEOSFeatures(llvm::Module&);
  void initializeGEOSBasicBlockFreqFeature(llvm::Module&);

}

#endif
