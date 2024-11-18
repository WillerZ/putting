cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Putting CXX)

# find_package(GMPXX)

include(CTest)

add_executable (putting putting.cpp)
target_link_libraries(putting gmpxx gmp)