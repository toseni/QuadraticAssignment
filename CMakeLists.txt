cmake_minimum_required(VERSION 3.7.2)
project(QuadraticAssignment C)

find_package(MPI REQUIRED)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "-fopenmp")

include_directories(${MPI_INCLUDE_PATH})

SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpicxx)

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
        Algorithm/simpleBranchAndBound.c Algorithm/parallelBranchAndBound.c DataStructure/LinkedStack.h DataStructure/ParallelStack.h DataStructure/ParallelStack.c DataStructure/LinkedQueue.c DataStructure/LinkedQueue.h MPI/mpiWrapper.c MPI/mpiWrapper.h Algorithm/hybridBranchAndBound.c)

add_executable(QuadraticAssignment ${SOURCE_FILES})

target_link_libraries(QuadraticAssignment m)