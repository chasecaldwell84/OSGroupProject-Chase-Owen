cmake_minimum_required(VERSION 3.28)
project(OperatingSystemsGroupProject)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenMP REQUIRED COMPONENTS CXX)

include_directories(.)
include_directories("Workload 1 - Independent Loop Computation")
include_directories("Workload 2 - Dot Product with Reduction")
include_directories("Workload 3 - Monte Carlo Estimate of Pi")

add_executable(OperatingSystemsGroupProject
        "Workload 1 - Independent Loop Computation/loop.cpp"
        "Workload 1 - Independent Loop Computation/loop.h"
        "Workload 2 - Dot Product with Reduction/dot.cpp"
        "Workload 2 - Dot Product with Reduction/dot.h"
        "Workload 3 - Monte Carlo Estimate of Pi/monte.cpp"
        "Workload 3 - Monte Carlo Estimate of Pi/monte.h"
        constants.cpp
        constants.h
        main.cpp)

target_link_libraries(OperatingSystemsGroupProject PRIVATE OpenMP::OpenMP_CXX)
target_compile_options(OperatingSystemsGroupProject PRIVATE -fpermissive)
