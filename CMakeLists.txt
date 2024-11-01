cmake_minimum_required(VERSION 3.22)
project(flowshop_ic)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -m64 -O -fPIC -fexceptions -DNDEBUG -DIL_STD -stdlib=libc++ -std=c++17")

add_executable(flowshop_ic
        Code/cplexModel/cplexModel.cpp
        Code/cplexModel.h
        Code/bits/stdc++.h
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

include_directories(/Applications/CPLEX_Studio2211/cplex/include)
include_directories(/Applications/CPLEX_Studio2211/cplex/include/ilcplex)
include_directories(/Applications/CPLEX_Studio2211/concert/include)
include_directories(/Applications/CPLEX_Studio2211/concert/include/ilconcert)

target_link_libraries(flowshop_ic PUBLIC /Applications/CPLEX_Studio2211/cplex/lib/arm64_osx/static_pic/libcplex.a)
target_link_libraries(flowshop_ic PUBLIC /Applications/CPLEX_Studio2211/cplex/lib/arm64_osx/static_pic/libilocplex.a)
target_link_libraries(flowshop_ic PUBLIC /Applications/CPLEX_Studio2211/concert/lib/arm64_osx/static_pic/libconcert.a)
set (target_link_options "-lconcert -lilocplex -lcplex -lm -lpthread -ldl -framework CoreFoundation -framework IOKit -stdlib=libc++")