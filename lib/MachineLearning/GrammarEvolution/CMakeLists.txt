add_library (Formula STATIC
  Formula.cpp
  LitFormula.cpp)

add_library (GrammarEvolution STATIC
  Candidate.cpp
  CandidateYAMLWrapper.cpp
  SimpleGrammarEvolution.cpp
  GEOSSimpleGrammarEvolution.cpp
  ParSimpleGrammarEvolution.cpp)
