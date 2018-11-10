//
// Created by tomas on 18.10.15.
//

#include <stdio.h>
#include "../DataStructure/DataStructure.h"
#include "../utils.h"

double assignmentCost(qa_global params, const int *assignment);

void swap(int *a, int *b);

stack_data randomWithCorrectingIterations(qa_global params) {
    stack_data heuristics;
    InitializeArray(params.locations, &heuristics.assignment);

    //random assignment
    for (int i = 0; i < params.locations; ++i) {
        heuristics.assignment[i] = i;
    }

    heuristics.cost = assignmentCost(params, heuristics.assignment);

    for (int i = 0; i < params.locations; ++i) {
        for (int j = 0; j < params.locations; ++j) {
            //swap factories
            swap(&heuristics.assignment[i], &heuristics.assignment[j]);

            double cost = assignmentCost(params, heuristics.assignment);

            if (cost > heuristics.cost) {
                swap(&heuristics.assignment[j], &heuristics.assignment[i]);
            } else {
                heuristics.cost = cost;
            }
        }
    }

    return heuristics;
}

double assignmentCost(qa_global params, const int *assignment) {
    double cost = 0;
    for (int i = 0; i < params.locations; ++i) {
        for (int j = i; j < params.locations; ++j) {
            cost += params.distanceMatrix[i][j] * params.flowMatrix[assignment[i]][assignment[j]];
        }
    }

    return cost;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}