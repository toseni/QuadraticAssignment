//
// Created by tomas on 18.10.7.
//

#ifndef QUADRATICASSIGMENT_DATASTRUCTURE_H
#define QUADRATICASSIGMENT_DATASTRUCTURE_H


typedef struct {
    int locations;
    int **distanceMatrix;
    int **flowMatrix;
} qa_global;

typedef struct {
    int x;
    int y;
} point;

typedef struct {
    int cost;
    int location;
    int* assignment;
    int* factoryTaken;
} stack_data;

typedef struct stack_node stack_node;
struct stack_node
{
    stack_data data;
    stack_node* next;
};

#endif //QUADRATICASSIGMENT_DATASTRUCTURE_H
