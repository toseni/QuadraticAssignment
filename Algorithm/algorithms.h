#ifndef QUADRATICASSIGNMENT_ALGORITHM_H
#define QUADRATICASSIGNMENT_ALGORITHM_H

#include "../DataStructure/DataStructure.h"

stack_data randomWithCorrectingIterations(qa_global params);
stack_data simpleBranchAndBound(qa_global params, stack_data heuristics);
stack_data parallelBranchAndBound(qa_global params, stack_data heuristics);
stack_data hybridBranchAndBound(qa_global params, stack_data heuristics);

#endif //QUADRATICASSIGNMENT_ALGORITHM_H
