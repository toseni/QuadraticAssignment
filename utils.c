//
// Created by tomas on 18.10.10.
//

#include <math.h>
#include "DataStructure/DataStructure.h"

double calculate_distance(point point1, point point2) {
    return sqrt(pow(point1.y - point2.y, 2) + pow(point1.x - point2.x, 2));
}

void PopulateDistanceMatrix(int n, point *array, double **distanceMatrix) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double temp = calculate_distance(array[i], array[j]);
            distanceMatrix[i][j] = temp;
        }
    }
}