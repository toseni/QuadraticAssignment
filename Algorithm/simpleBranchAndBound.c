//
// Created by tomas on 18.10.17.
//

#include <stddef.h>
#include <malloc.h>
#include "../DataStructure/DataStructure.h"
#include "../DataStructure/LinkedStack.h"
#include "../utils.h"

static int AllLocationsAssigned(int locations, int current);

static int CostOfAssignment(qa_global params, stack_data node, int currentLocation, int factory);

static int GetLowerBound(qa_global params, stack_data node);

stack_data simpleBranchAndBound(qa_global params, stack_data bestKnown) {
    stack_node *stack = NULL;
    init(stack);

    stack_data initialProblem;
    InitializeArray(params.locations, &initialProblem.assignment);
    InitializeArray(params.locations, &initialProblem.factoryTaken);
    initialProblem.cost = 0;
    initialProblem.location = -1;

    stack = push(stack, initialProblem);

    while (!isEmpty(stack)) {
        stack_data problem;
        stack = pop(stack, &problem);

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
                    printf("Better solution found: %d\n", subProblem.cost);
                    bestKnown.cost = subProblem.cost;
                    CopyArray(params.locations, subProblem.assignment, bestKnown.assignment);
                }

                continue;
            }

            double costEstimate = subProblem.cost + GetLowerBound(params, subProblem);
            if (bestKnown.cost < costEstimate) {
                continue;
            }

            stack = push(stack, subProblem);
        }

        free(problem.assignment);
        free(problem.factoryTaken);
    }

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