/*-------------------------- PINHAO project --------------------------*/

/**
 * @file YAMLWrapper.cpp
 */

#include "pinhao/Support/YAMLWrapper.h"
#include "pinhao/Support/YamlOptions.h"

using namespace pinhao;

static config::YamlOpt<bool> PrintReduced
("print-reduced", "Print all the informations in a reduced manner.", false, true);

static config::YamlOpt<bool> PrintComments
("print-comments", "Print the description, if defined.", false, false);

/*
 * -----------------------------------
 *  Overloading for: void*
 */
template<> void pinhao::YAMLWrapper::fill<void*>(void *&Vd, ConstNode &Node) {
  Vd = (void*) Node.as<uint64_t>();
}

template<> void pinhao::YAMLWrapper::append<void*>(void* const &Vd, Emitter &E) {
  E << (uint64_t) Vd;
}
