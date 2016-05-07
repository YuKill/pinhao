/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Feature.cpp
 */

#include "pinhao/Features/Feature.h"

using namespace pinhao;

CompositeFeatureInfo *Feature::getCompositeFeatureInfo() const {
  if (!isComposite()) return nullptr;
  return static_cast<CompositeFeatureInfo*>(Info.get());
}

Feature::Feature(FeatureInfo* InfoPtr, FeatureKind FKind) : 
  Info(std::shared_ptr<FeatureInfo>(InfoPtr)), Kind(FKind) { 
    Processed = false; 
  }

std::string Feature::getName() const { 
  return Info->getName(); 
}

std::string Feature::getDescription() const { 
  return Info->getDescription(); 
}

ValueType Feature::getType() const { 
  return Info->getType();
}

bool Feature::hasSubFeature(std::string SubFeatureName) const {
  if (!isComposite()) return false;
  return getCompositeFeatureInfo()->hasSubFeature(SubFeatureName);
}

std::string Feature::getSubFeatureDescription(std::string SubFeatureName) const {
  assert(hasSubFeature(SubFeatureName) && "This feature does not have the sub-feature desired.");
  return getCompositeFeatureInfo()->getSubFeatureDescription(SubFeatureName); 
}

uint64_t Feature::getNumberOfSubFeatures() const {
  if (!isComposite()) return 0;
  return getCompositeFeatureInfo()->getNumberOfSubFeatures();  
}

FeatureKind Feature::getKind() const { 
  return Kind;
}

bool Feature::isComposite() const { 
  return Info->isComposite();
}

bool Feature::isProcessed() const { 
  return Processed;
}

bool Feature::isLinear() const { 
  return Kind == FeatureKind::LinearKind;
}

bool Feature::isMapped() const { 
  return Kind == FeatureKind::MappedKind;
}

bool Feature::isStaticFeature() const { 
  return Info->isStaticFeature();
}

bool Feature::isDynamicFeature() const { 
  return Info->isDynamicFeature();
}

Feature::iterator Feature::begin() const {
  return getCompositeFeatureInfo()->begin();
}

Feature::iterator Feature::end() const {
  return getCompositeFeatureInfo()->end();
}

void Feature::print(std::ostream &Out) {
  YAML::Emitter Emitter(Out);
  append(Emitter);
  Out << std::endl;
}
