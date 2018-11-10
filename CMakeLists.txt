cmake_minimum_required(VERSION 3.7.2)
project(QuadraticAssignment C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "-fopenmp")

set(SOURCE_FILES
        main.c
        DataStructure/DataStructure.h
        DataStructure/LinkedStack.c
        IO/IO.h
        IO/IO.c
        utils.h
        utils.c
        Algorithm/heuristics.c
        Algorithm/algorithms.h
        Algorithm/simpleBranchAndBound.c)

add_executable(QuadraticAssignment ${SOURCE_FILES})

target_link_libraries(QuadraticAssignment m)