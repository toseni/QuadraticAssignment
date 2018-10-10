//
// Created by tomas on 18.10.10.
//

#include <bits/types/FILE.h>
#include <stdio.h>
#include <stdlib.h>
#include "../DataStructure/DataStructure.h"
#include "../utils.h"

void ExtractCoordinates(FILE *fp, point cities[]) {
    char buffer[255];
    int i = 0;
    while (1) {
        fgets(buffer, 255, fp);

        if (buffer[0] == '#') {
            break;
        }

        sscanf(buffer, "%*d %d %d\n", &(cities[i].x), &(cities[i].y));
        i++;
    }
}

qa_global ReadGlobalData(const char *fileName)
{
    FILE *fp;
    fp = fopen(fileName, "r");

    int locations;

    fscanf(fp, "# Locations: %d\n", &locations);

    qa_global params;
    params.locations = locations;

    point points[params.locations];
    ExtractCoordinates(fp, points);

    params.distanceMatrix = (double **)malloc(sizeof(double *) * params.locations);
    for (int j = 0; j < params.locations; j++)
        params.distanceMatrix[j] = (double *)malloc(params.locations * sizeof(double));

    PopulateDistanceMatrix(params.locations, points, params.distanceMatrix);

    params.flowMatrix = (int **)malloc(sizeof(int *) * params.locations);
    for (int j = 0; j < params.locations; j++)
        params.flowMatrix[j] = (int *)malloc(params.locations * sizeof(int));

    while (!feof (fp)) {

        for (int i = 0; i < params.locations; ++i) {
            for (int j = 0; j < params.locations; ++j) {
                fscanf(fp, "%d", &(params.flowMatrix[i][j]));
            }
        }
    }

    fclose(fp);

    return params;
}