
# Grouping libs.
## geos libs
set (GEOS_LIBS "-lGEOSMain -lGEOSCallCostReader -lGEOSProf -lGEOSProfileModule -lGEOSJITExecution -lGEOSCostEstimator")

## static-profiler libs
set (SPROF_LIBS "-lStaticProfiler")

## llvm-config --libs all
execute_process (COMMAND llvm-config --libs all --system-libs OUTPUT_VARIABLE LLVM_LIBS)
string (REPLACE "\n" " " LLVM_LIBS "${LLVM_LIBS}")
string (REGEX REPLACE " $" "" LLVM_LIBS "${LLVM_LIBS}")

## google-test
set (GTEST_LIBS "-lgtest -lgtest_main")

## other minor libs
### - yaml-cpp
### - papi
set (UTIL_LIBS "-lyaml-cpp -lpapi")

# Functions
## Function for linking tests.
function (pinhao_test_link TestName)
  pinhao_link_local (${TestName})
  target_link_libraries (${TestName} ${GTEST_LIBS})
  pinhao_link_toolset (${TestName})
endfunction (pinhao_test_link)

## Function for linking tools.
function (pinhao_tool_link ToolName)
  pinhao_link_local (${ToolName})
  pinhao_link_toolset (${ToolName})
endfunction (pinhao_tool_link)

## Function for linking shared libraries (llvm passes).
function (pinhao_pass_link PassName)
  pinhao_link_local (${PassName})
  target_link_libraries (${PassName} ${UTIL_LIBS})
endfunction (pinhao_pass_link)

## Function for linking the toolset library.
function (pinhao_link_toolset Target)
  target_link_libraries (${Target}
    ${UTIL_LIBS} 
    ${SPROF_LIBS} 
    ${GEOS_LIBS} 
    ${LLVM_LIBS})
endfunction (pinhao_link_toolset)

## Function for linking the local library.
function (pinhao_link_local Target)
  target_link_libraries (${Target}
    Features Optimizer Formula 
    GEOSWrapper SProfWrapper PAPIWrapper 
    Support)
endfunction(pinhao_link_local)
