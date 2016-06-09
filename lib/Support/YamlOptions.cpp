/*-------------------------- PINHAO project --------------------------*/

/**
 * @file YamlOptions.cpp
 */

#include "pinhao/Support/YamlOptions.h"

#include <map>

using namespace pinhao;

/*
 * ----------------------------------=
 * Class: YamlOptions
 */
namespace {
  
  class YamlOptions {
    private:
      typedef config::YamlOptBase Option;
      typedef std::map<std::string, std::map<uint64_t, Option*>> OptionsMap;

      static void removeIfEmpty(std::string OptionName);
      static OptionsMap &getOptions();

    public:
      static void add(Option *O); 
      static void remove(std::string OptionName);
      static void remove(Option *O);

      static bool isRegistered(std::string OptionName);
      static bool isRegistered(Option &O);

      static void parse(const YAML::Node &Node);
      static bool checkRequiredOptions();
  };

}

void YamlOptions::removeIfEmpty(std::string OptionName) {
  OptionsMap &Options = getOptions();
  if (Options[OptionName].empty())
    Options.erase(OptionName);
}

YamlOptions::OptionsMap &YamlOptions::getOptions() {
  static std::shared_ptr<OptionsMap> Options(nullptr);

  if (Options.get() == nullptr)
    Options.reset(new OptionsMap);

  return *Options;
}

void YamlOptions::add(Option *O) {
  OptionsMap &Options = getOptions();
  Options[O->getName()].insert(std::make_pair((uint64_t)O, O));
}

void YamlOptions::remove(std::string OptionName) {
  if (isRegistered(OptionName))
    getOptions().erase(OptionName);
}

void YamlOptions::remove(Option *O) {
  if (!isRegistered(*O)) return;

  OptionsMap &Options = getOptions();
  Options[O->getName()].erase((uint64_t)O);

  removeIfEmpty(O->getName());
}

bool YamlOptions::isRegistered(std::string OptionName) {
  return getOptions().count(OptionName) > 0;
}

bool YamlOptions::isRegistered(Option &O) {
  OptionsMap &Options = getOptions();

  return Options.count(O.getName()) > 0 &&
    Options[O.getName()].count((uint64_t)&O) > 0;
}

void YamlOptions::parse(const YAML::Node &Node) {
  for (auto I = Node.begin(), E = Node.end(); I != E; ++I) {
    std::string OptName = I->first.as<std::string>();

    if (isRegistered(OptName)) {
      OptionsMap &Options = getOptions();

      for (auto &OptPair : Options[OptName])
        OptPair.second->parse(I->second);
    }
  }
}

bool YamlOptions::checkRequiredOptions() {
  OptionsMap &Options = getOptions();

  for (auto &Pair : Options) 
    for (auto &OptPair : Pair.second) {
      if (!OptPair.second->isRequired()) break;
      if (!OptPair.second->hasParsed())
        return false;
    }

  return true;
}

/*
 * ----------------------------------=
 * Config namespace functions
 */
bool config::isRegistered(std::string OptName) {
  return YamlOptions::isRegistered(OptName);
}

void config::parseOptions(std::string Filename) {
  YAML::Node Node = YAML::LoadFile(Filename); 
  parseOptions(Node);
}

void config::parseOptions(YAML::Node &Node) {
  YamlOptions::parse(Node);
  assert(YamlOptions::checkRequiredOptions() && "Required options not found in config file."); 
}

void config::printHelp(std::ostream &Out) {

}

/*
 * ----------------------------------=
 * Class: YamlOptBase
 */
#include <iostream>

config::YamlOptBase::~YamlOptBase() {
  unregister();
}

config::YamlOptBase::YamlOptBase(std::string Name, std::string Description, bool Required) : 
  Name(Name), Description(Description), Required(Required), Parsed(false) {
    registerOption();
  }

void config::YamlOptBase::registerOption() {
  YamlOptions::add(this);
}

void config::YamlOptBase::unregister() {
  YamlOptions::remove(this);
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
