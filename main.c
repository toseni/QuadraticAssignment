#include <stdio.h>
#include <omp.h>
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

    double start = omp_get_wtime();
    stack_data solution = parallelBranchAndBound(params, bestHeuristics);
    double duration = omp_get_wtime() - start;

    printf("It took %f seconds for algorithm.\n", duration);

    PrintResult(params, solution);

    return 0;
}