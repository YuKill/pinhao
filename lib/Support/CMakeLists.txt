add_subdirectory (YAMLWrapper)

add_library (Support STATIC
  Types.cpp
  YamlOptions.cpp
  Random.cpp
  JITExecutor.cpp
  $<TARGET_OBJECTS:YAMLWrapper>)
