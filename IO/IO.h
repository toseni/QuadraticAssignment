#ifndef QUADRATICASSIGMENT_IO_H
#define QUADRATICASSIGMENT_IO_H

#include "../DataStructure/DataStructure.h"

qa_global ReadGlobalData(const char *dataFileName);
void PrintResult(qa_global params, stack_data solution);

#endif //QUADRATICASSIGMENT_IO_H
