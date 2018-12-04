//
// Created by tomas on 18.12.4.
//

#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include "DataStructure.h"

static stack_node *_head;

static omp_lock_t _lock;
static int _numberOfThreads;

static volatile int *_workFlags;

void initStackParallel() {
    _head = NULL;

    _numberOfThreads = omp_get_num_threads();

    _workFlags = (int *) malloc(sizeof(int) * _numberOfThreads);
    for (int i = 0; i < _numberOfThreads; ++i) {
        _workFlags[i] = 0;
    }

    omp_init_lock(&_lock);
}

void pushParallel(stack_data data) {
    stack_node *tmp = (stack_node *) malloc(sizeof(stack_node));
    if (tmp == NULL) {
        exit(111);
    }

    omp_set_lock(&_lock);

    tmp->data = data;
    tmp->next = _head;
    _head = tmp;

    omp_unset_lock(&_lock);
}

int popParallel(stack_data *element) {
    omp_set_lock(&_lock);

    if (_head == NULL) {
        _workFlags[omp_get_thread_num()] = 0;

        omp_unset_lock(&_lock);
        return 0;
    }

    stack_node *tmp = _head;
    *element = _head->data;
    _head = _head->next;

    free(tmp);

    _workFlags[omp_get_thread_num()] = 1;
    omp_unset_lock(&_lock);

    return 1;
}

int isEmptyParallel() {
    omp_set_lock(&_lock);

    int isEmpty = _head == NULL ? 1 : 0;
    if (isEmpty) {
        for (int i = 0; i < _numberOfThreads; ++i) {
            if (_workFlags[i]) {
                omp_unset_lock(&_lock);
                return 0;
            }
        }

        omp_unset_lock(&_lock);
        return 1;
    }

    _workFlags[omp_get_thread_num()] = 1;
    omp_unset_lock(&_lock);
    return 0;
}

void destroyStack() {
    omp_destroy_lock(&_lock);
    free((void*)_workFlags);
}