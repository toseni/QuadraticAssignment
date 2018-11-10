//
// Created by tomas on 18.10.17.
//

#include <stddef.h>
#include <malloc.h>
#include "../DataStructure/DataStructure.h"
#include "../utils.h"

int allLocationsAssigned(qa_global params, stack_data node);

double costOfAssignment(qa_global params, stack_data node, int location, int factory);

int getLowerBound(qa_global params, stack_data node);

stack_data simpleBranchAndBound(qa_global params, stack_data bestKnown) {
    stack_node *stack = NULL;
    init(stack);

    stack_data initialProblem;
    InitializeArray(params.locations, &initialProblem.assignment);
    InitializeArray(params.locations, &initialProblem.factoryTaken);
    InitializeArray(params.locations, &initialProblem.locationTaken);
    initialProblem.cost = 0;

    stack = push(stack, initialProblem);

    while (!isEmpty(stack)) {
        stack_data problem;
        stack = pop(stack, &problem);

        for (int i = 0; i < params.locations; ++i) {
            if (problem.locationTaken[i]) {
                continue;
            }

            for (int j = 0; j < params.locations; ++j) {
                if (problem.factoryTaken[i]) {
                    continue;
                }

                stack_data subProblem;
                InitializeAndCopyArray(params.locations, problem.assignment, &subProblem.assignment);
                InitializeAndCopyArray(params.locations, problem.factoryTaken, &subProblem.factoryTaken);
                InitializeAndCopyArray(params.locations, problem.locationTaken, &subProblem.locationTaken);
                subProblem.locationTaken[i] = 1;
                subProblem.factoryTaken[j] = 1;
                subProblem.assignment[i] = j;
                subProblem.cost += costOfAssignment(params, subProblem, i, j);

                if (allLocationsAssigned(params, subProblem)) {
                    if (bestKnown.cost >= subProblem.cost) {
                        printf("Better solution found: %f\n", subProblem.cost);
                        bestKnown.cost = subProblem.cost;
                        CopyArray(params.locations, subProblem.assignment, bestKnown.assignment);
                    }

                    continue;
                }

                double costEstimate = subProblem.cost + getLowerBound(params, subProblem);
                if (bestKnown.cost < costEstimate) {
                    continue;
                }

                stack = push(stack, subProblem);
            }
        }

        free(problem.assignment);
        free(problem.factoryTaken);
        free(problem.locationTaken);
    }

    return bestKnown;
}

double costOfAssignment(qa_global params, stack_data node, int location, int factory) {
    double cost = 0;
    for (int i = 0; i < params.locations; ++i) {
        if (!node.locationTaken[i]) {
            continue;
        }

        for (int j = i; j < params.locations; ++j) {
            if (!node.factoryTaken[j]) {
                continue;
            }

            cost += params.distanceMatrix[location][i] * params.flowMatrix[factory][node.assignment[j]];
        }
    }

    return cost;
}

int allLocationsAssigned(qa_global params, stack_data node) {
    for (int i = 0; i < params.locations; ++i) {
        if (node.locationTaken[i] == 0) {
            return 0;
        }
    }

    return 1;
}

int getLowerBound(qa_global params, stack_data node) {
    return 0;
}