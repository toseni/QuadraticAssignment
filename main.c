#include <stdio.h>
#include "DataStructure/DataStructure.h"
#include "IO/IO.h"

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Usage: ./QuadraticAssignment problemFileName\n");
        return 1;
    }

    char* dataFileName = argv[1];

    qa_global params = ReadGlobalData(dataFileName);

    printf("Distance matrix:\n");
    for (int i = 0; i < params.locations; ++i) {
        for (int j = 0; j < params.locations; ++j) {
            printf("%6f ", params.distanceMatrix[i][j]);
        }
        printf("\n");
    }

    printf("Flow matrix:\n");
    for (int i = 0; i < params.locations; ++i) {
        for (int j = 0; j < params.locations; ++j) {
            printf("%4d ", params.flowMatrix[i][j]);
        }
        printf("\n");
    }


    return 0;
}