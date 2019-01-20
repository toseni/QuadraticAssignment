#include <stdio.h>
#include <float.h>
#include <omp.h>
#include <malloc.h>
#include <mpi.h>
#include <stdlib.h>
#include "../utils.h"
#include "../DataStructure/DataStructure.h"
#include "../DataStructure/ParallelStack.h"
#include "../DataStructure/LinkedQueue.h"
#include "../MPI/mpiWrapper.h"

static int AllLocationsAssigned(int locations, int current);

static int CostOfAssignment(qa_global params, stack_data node, int currentLocation, int factory);

static int GetLowerBound(qa_global params, stack_data node);

stack_data hybridBranchAndBound(qa_global params, stack_data bestKnown) {

    int size, rank;
    MPI_Wrapper_Init(params.locations, &size, &rank);

    stack_data initialProblem;
    InitializeArray(params.locations, &initialProblem.assignment);
    InitializeArray(params.locations, &initialProblem.factoryTaken);
    initialProblem.cost = 0;
    initialProblem.location = -1;

    //split init problem
    initQueue();
    enQueue(initialProblem);

    while (queueSize() < 10 * omp_get_max_threads() * size) {
        stack_data problem = deQueue();

        int current = ++problem.location;

        for (int i = 0; i < params.locations; ++i) {
            if (problem.factoryTaken[i]) {
                continue;
            }

            stack_data subProblem;
            subProblem.location = current;
            InitializeAndCopyArray(params.locations, problem.assignment, &subProblem.assignment);
            subProblem.assignment[current] = i;
            InitializeAndCopyArray(params.locations, problem.factoryTaken, &subProblem.factoryTaken);
            subProblem.factoryTaken[i] = 1;
            subProblem.cost = problem.cost + CostOfAssignment(params, subProblem, current, i);

            if (AllLocationsAssigned(params.locations, current)) {
                if (bestKnown.cost >= subProblem.cost) {
                    #pragma omp critical
                    {
                        if (bestKnown.cost >= subProblem.cost) {
                            bestKnown.cost = subProblem.cost;
                            CopyArray(params.locations, subProblem.assignment, bestKnown.assignment);
                        }
                    }
                }

                free(subProblem.assignment);
                free(subProblem.factoryTaken);
                continue;
            }

            double costEstimate = subProblem.cost + GetLowerBound(params, subProblem);
            if (bestKnown.cost < costEstimate) {

                free(subProblem.assignment);
                free(subProblem.factoryTaken);
                continue;
            }

            enQueue(subProblem);
        }

        free(problem.assignment);
        free(problem.factoryTaken);
    }

    // create stack
    initStackParallel();

    // distribute
    int count = 0;
    while (!isQueueEmpty()) {
        int left = queueSize();
        stack_data problem = deQueue();

        int destination = left % size;
        if (destination == rank) {
            count++;
            pushParallel(problem);
        }
    }

    printf("I am process %d with %d threads and have stack of size %d\n", rank, omp_get_max_threads(), count);

    #pragma omp parallel
    {
        while (!isEmptyParallel()) {
            stack_data problem;
            int success = popParallel(&problem);
            if (!success) {
                continue;
            }

            int current = ++problem.location;

            for (int i = 0; i < params.locations; ++i) {
                if (problem.factoryTaken[i]) {
                    continue;
                }

                stack_data subProblem;
                subProblem.location = current;
                InitializeAndCopyArray(params.locations, problem.assignment, &subProblem.assignment);
                subProblem.assignment[current] = i;
                InitializeAndCopyArray(params.locations, problem.factoryTaken, &subProblem.factoryTaken);
                subProblem.factoryTaken[i] = 1;
                subProblem.cost = problem.cost + CostOfAssignment(params, subProblem, current, i);

                if (AllLocationsAssigned(params.locations, current)) {
                    if (bestKnown.cost >= subProblem.cost) {
                        #pragma omp critical
                        {
                            if (bestKnown.cost >= subProblem.cost) {
                                bestKnown.cost = subProblem.cost;
                                CopyArray(params.locations, subProblem.assignment, bestKnown.assignment);
                            }
                        }
                    }

                    free(subProblem.assignment);
                    free(subProblem.factoryTaken);
                    continue;
                }

                double costEstimate = subProblem.cost + GetLowerBound(params, subProblem);
                if (bestKnown.cost < costEstimate) {

                    free(subProblem.assignment);
                    free(subProblem.factoryTaken);
                    continue;
                }

                pushParallel(subProblem);
            }

            free(problem.assignment);
            free(problem.factoryTaken);
        }
    }

    destroyStack();

    MPI_Wrapper_Synchronize(&bestKnown);
    printf("Finished work on process %d\n", rank);

    MPI_Wrapper_Finalize();

    return bestKnown;
}

static int CostOfAssignment(qa_global params, stack_data node, int currentLocation, int factory) {
    int cost = 0;
    for (int i = 0; i < currentLocation; ++i) {
        cost += params.distanceMatrix[i][currentLocation] * params.flowMatrix[node.assignment[i]][factory];
    }

    return cost;
}

static int AllLocationsAssigned(int locations, int current) {
    return (current + 1) >= locations;
}

static int GetLowerBound(qa_global params, stack_data node) {
    return 0;
}
