#include <stdio.h>
#include "DataStructure/DataStructure.h"
#include "IO/IO.h"
#include "Algorithm/algorithms.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: ./QuadraticAssignment problemFileName\n");
        return 1;
    }

    char *dataFileName = argv[1];

    qa_global params = ReadGlobalData(dataFileName);

    stack_data bestHeuristics = randomWithCorrectingIterations(params);

    printf("Solution cost: %f\n", bestHeuristics.cost);
    printf("Assignment: ");
    for (int i = 0; i < params.locations; ++i) {
        printf("%d:%d ", i, bestHeuristics.assignment[i]);
    }
    printf("\n");

    stack_data solution = simpleBranchAndBound(params, bestHeuristics);

    //PrintResult(params, solution);

    printf("Solution cost: %f\n", solution.cost);
    printf("Assignment: ");
    for (int i = 0; i < params.locations; ++i) {
        printf("%d:%d ", i, solution.assignment[i]);
    }

    return 0;
}