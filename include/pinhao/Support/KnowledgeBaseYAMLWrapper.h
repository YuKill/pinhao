/*-------------------------- PINHAO project --------------------------*/

/**
 * @file KnowledgeBaseYAMLWrapper.h
 */

#ifndef PINHAO_KNOWLEDGE_BASE_YAML_WRAPPER_H
#define PINHAO_KNOWLEDGE_BASE_YAML_WRAPPER_H

template <class T, class U>
void pinhao::YAMLWrapper::fill(KnowledgeBase<T, U> &Value, ConstNode &Node) {
  for (auto I = Node.begin(), E = Node.end(); I != E; ++I) {
    T Data;
    YAMLWrapper::fill(Data, *I);
    Value.insert(Data);
  }
}

template <class T, class U>
void pinhao::YAMLWrapper::append(const KnowledgeBase<T, U> &Value, Emitter &E) {
  E << YAML::BeginSeq;
  for (auto &Data : Value)
    YAMLWrapper::append(Data, E); 
  E << YAML::EndSeq;
}

#endif
