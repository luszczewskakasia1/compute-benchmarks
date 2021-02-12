#include "framework/argument/arguments.h"
#include "framework/argument/basic_argument.h"

struct WorkloadParameters : ArgumentsBase {
    PositiveIntegerArgument iterations;
    BooleanArgument synchronize;

    WorkloadParameters()
        : iterations(*this, "iterations", "Number of iterations to perform"),
          synchronize(*this, "synchronize", "Wait for synchronization before each iteration") {}
};
