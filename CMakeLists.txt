cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Putting CXX)

# find_package(GMPXX)

include(CTest)

add_library(stats stats.cpp)
target_link_libraries(stats -static gmpxx gmp)
add_executable (1tail 1tail.cpp)
target_link_libraries(1tail -static stats)
