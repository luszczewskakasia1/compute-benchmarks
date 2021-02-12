#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/test_case_argument/test_case_arguments.h"

struct WorkloadParameters : ArgumentsBase {
    PositiveIntegerArgument iterations;
    BooleanArgument synchronize;

    WorkloadParameters()
        : iterations(*this, "iterations", "Number of iterations to perform"),
          synchronize(*this, "synchronize", "Wait for synchronization before each iteration") {}
};
