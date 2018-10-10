# Quadratic Assignment

Quadratic Assignment solver implementation using branch and bound algorithm.

## Getting started

This instruction will help you to install and run this solver.

### Prerequisites

`C` compiler supporting `OpenMP`

```cmake``` 

`python3` if you want to generate random problems

### Building

1. Create directory for build files. 

```$ mkdir debug-build```

2. Enter the directory.

```$ cd debug-build```

3. Run `cmake`

```$ cmake ../```

For release build run 

```$ cmake -DCMAKE_BUILD_TYPE=Release ../```

4. Run `Makefile`

```$ make```

### Running 

To run build application call executable and pass file with problem data.

```$ ./QuadraticAssigment ../qa_data/qa_10.out```