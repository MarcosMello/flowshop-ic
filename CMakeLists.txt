cmake_minimum_required(VERSION 3.22)
project(flowshop_ic)

set(CMAKE_CXX_COMPILER g++)

set(CPLEX_ROOT_DIR                 "/opt/ibm/ILOG/CPLEX_Studio2211")
set(CMAKE_BUILD_TYPE               "Debug")
set(CMAKE_MODULE_PATH               "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS                 "${CMAKE_CXX_FLAGS} -std=c++17 -DIL_STD -m64 -Wall -Werror")
set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_CXX_FLAGS_DEBUG} -Og -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -O3 -flto")
set(CMAKE_INCLUDE_SYSTEM_FLAG_CXX   "-isystem ")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${CMAKE_CURRENT_SOURCE_DIR}/build")
set(LINKER_OPTIONS                  -flto -Wl,--no-as-needed)

find_package(Cplex)
include_directories(SYSTEM ${CPLEX_INCLUDE_DIRS})

add_executable(flowshop_ic
        Code/cplexModel/cplexModel.cpp
        Code/cplexModel.h
        Code/GeneticAlgorithm/GeneticAlgorithm.cpp
        Code/inputData/inputData.cpp
        Code/inputData.h
        Code/GeneticAlgorithm.h
        Code/GeneticAlgorithm/Population.cpp
        Code/GeneticAlgorithm/Individual.cpp
        Code/GeneticAlgorithm/Conclusion.cpp
        Code/main.cpp
        Code/Latex/comparisonTable.cpp
        Code/TabuSearch/TabuSearch.cpp
        Code/NEH.h
        Code/Heuristic/NEH.cpp
)

target_link_libraries(flowshop_ic PRIVATE ${CPLEX_LIBRARIES})
target_link_libraries(flowshop_ic PRIVATE dl)