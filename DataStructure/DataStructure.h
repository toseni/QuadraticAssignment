//
// Created by tomas on 18.10.7.
//

#ifndef QUADRATICASSIGMENT_DATASTRUCTURE_H
#define QUADRATICASSIGMENT_DATASTRUCTURE_H


typedef struct {
    int locations;
    double **distanceMatrix;
    int **flowMatrix;
} qa_global;

typedef struct {
    int x;
    int y;
} point;

typedef struct {

} stack_data;

typedef struct stack_node stack_node;
struct stack_node
{
    stack_data data;
    stack_node* next;
};

int isEmpty(stack_node *s);
stack_node* push(stack_node *s, stack_data data);
stack_node* pop(stack_node *s, stack_data *data);
void init(stack_node* s);

#endif //QUADRATICASSIGMENT_DATASTRUCTURE_H
