/*-------------------------- PINHAO project --------------------------*/

/**
 * @file YamlOptions.cpp
 */

#include "pinhao/Support/YamlOptions.h"

#include <map>

using namespace pinhao;

namespace pinhao {
  namespace config {
    static std::map<std::string, std::map<uint64_t, YamlOptBase*>> Options;
  }
}

static bool areRequiredOptsParsed() {
  for (auto &Pair : config::Options) 
    for (auto &OptPair : Pair.second)
      if (OptPair.second->isRequired() && !OptPair.second->hasParsed())
        return false;
  return true;
}

bool config::isRegistered(std::string OptName) {
  return Options.count(OptName) > 0;
}

void config::parseOptions(std::string Filename) {
  YAML::Node Node = YAML::LoadFile(Filename); 
  parseOptions(Node);
}

void config::parseOptions(YAML::Node &Node) {
  for (auto I = Node.begin(), E = Node.end(); I != E; ++I) {
    std::string OptName = I->first.as<std::string>();
    if (isRegistered(OptName)) {
      for (auto &OptPair : config::Options[OptName])
        OptPair.second->parse(I->second);
    }
  }
  assert(areRequiredOptsParsed() && "Required options not found."); 
}

void config::printHelp(std::ostream &Out) {

}


/*
 * ----------------------------------=
 * Class: YamlOptBase
 */
config::YamlOptBase::~YamlOptBase() {
  unregister();
}

config::YamlOptBase::YamlOptBase(std::string Name, std::string Description, bool Required) : 
  Name(Name), Description(Description), Required(Required), Parsed(false) {
    registerOption();
  }

void config::YamlOptBase::registerOption() {
  Options[Name].insert(std::make_pair((uint64_t)this, this));
}

void config::YamlOptBase::unregister() {
  if (isRegistered(Name) && Options[Name].count((uint64_t)this) > 0)
    Options[Name].erase((uint64_t)this);
}

std::string config::YamlOptBase::getName() {
  return Name;
}

bool config::YamlOptBase::isRequired() {
  return Required;
}

bool config::YamlOptBase::hasParsed() {
  return Parsed;
}
