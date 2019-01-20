#include "mpiWrapper.h"
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

static MPI_Datatype _stackDataType;
static int _problemSize;
static int _rank;
static int _size;
static int _boundSource;

static int FindBoundSource();

static MPI_Datatype MpiStackData() {
    MPI_Datatype mpi_stack_type;

    MPI_Datatype types[2] = {MPI_INT, MPI_INT};
    int blocklengths[2] = {1, 1};
    MPI_Aint offsets[2];

    offsets[0] = offsetof(stack_data, cost);
    offsets[1] = offsetof(stack_data, location);

//    TODO improve displacement calculation. Might be what is causing crashes. ???
//    MPI_Get_Address();
//    MPI_Get_Address(ant[0].tour,disp+1);
//    MPI_Get_Address(ant[0].visited,disp+2);
//    base=disp[0];
//    int i;
//    for(i=0;i<3;i++)
//    {
//        disp[i]-=base;
//    }

    MPI_Type_create_struct(3, blocklengths, offsets, types, &mpi_stack_type);
    MPI_Type_commit(&mpi_stack_type);

    return mpi_stack_type;
}

void MPI_Wrapper_Init(int problemSize, int *size, int *rank) {
    int provided;
    MPI_Init_thread(NULL, NULL, MPI_THREAD_SERIALIZED, &provided);

    MPI_Comm_size(MPI_COMM_WORLD, size);
    MPI_Comm_rank(MPI_COMM_WORLD, rank);

    if (*rank == 0) {
        printf("Provided thread support level: %d\n", provided);
        if (provided != MPI_THREAD_SERIALIZED) {
            exit(1234);
        }
    }

    _problemSize = problemSize;
    _size = *size;
    _rank = *rank;

    _stackDataType = MpiStackData();
    _boundSource = _rank;
}

void MPI_Wrapper_Finalize() {
    MPI_Type_free(&_stackDataType);

    MPI_Finalize();
}

void MPI_Wrapper_Send_Problem(stack_data node, int destination) {

    MPI_Send(&node, 1, _stackDataType, destination, MPI_PROBLEM_TAG, MPI_COMM_WORLD);
    MPI_Send(node.assignment, _problemSize, MPI_INT, destination, MPI_PROBLEM_TAG, MPI_COMM_WORLD);
    MPI_Send(node.factoryTaken, _problemSize, MPI_INT, destination, MPI_PROBLEM_TAG, MPI_COMM_WORLD);
}

stack_data MPI_Wrapper_Receive_Problem() {

    stack_data node;
    node.assignment = malloc(_problemSize * sizeof(int));

    MPI_Recv(&node, 1, _stackDataType, _rank, MPI_PROBLEM_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(node.assignment, _problemSize, MPI_INT, _rank, MPI_PROBLEM_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(node.factoryTaken, _problemSize, MPI_INT, _rank, MPI_PROBLEM_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    return node;
}

void MPI_Wrapper_Receive_Bound(stack_data *bound) {
    int flag;
    MPI_Status probeStatus;
    MPI_Iprobe(MPI_ANY_SOURCE, MPI_BOUND_TAG, MPI_COMM_WORLD, &flag, &probeStatus);

    if (flag) {
        MPI_Status status;
        long externalBound;
        MPI_Recv(&externalBound, 1, MPI_LONG, probeStatus.MPI_SOURCE, MPI_BOUND_TAG, MPI_COMM_WORLD, &status);
        if (bound->cost >= externalBound) {
            bound->cost= externalBound;
            _boundSource = status.MPI_SOURCE;
        }

        MPI_Wrapper_Receive_Bound(bound);
    }
}

void MPI_Wrapper_Share_Bound(stack_data bound) {
    for (int destination = 0; destination < _size; ++destination) {
        if (destination == _rank) continue;

        MPI_Send(&bound.cost, 1, MPI_LONG, destination, MPI_BOUND_TAG, MPI_COMM_WORLD);
    }

    _boundSource = _rank;
}

void MPI_Wrapper_Synchronize(stack_data *bound) {
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Wrapper_Receive_Bound(bound);

    MPI_Barrier(MPI_COMM_WORLD);
    int boundSource = FindBoundSource();
    MPI_Bcast(bound->assignment, _problemSize, MPI_INT, boundSource, MPI_COMM_WORLD);
}

static int FindBoundSource() {
    int *boundSources = NULL;
    if (_rank == 0) {
        boundSources = malloc(sizeof(int) * _size);
    }

    MPI_Gather(&_boundSource, 1, MPI_INT, boundSources, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (_rank == 0) {
        int failed = 0;
        for (int i = 0; i < _size - 1; ++i) {
            if (boundSources[i] != boundSources[i + 1]) {
                failed = 1;
            }
        }

        if (failed) {
            printf("Best bound sources does not match. ");
            for (int i = 0; i < _size; ++i) {
                printf("%d ", boundSources[i]);
            }
            printf("\n");

            exit(2345);
        }
    }

    return _boundSource;
}
