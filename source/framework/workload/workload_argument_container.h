#include "framework/argument/argument_container.h"
#include "framework/argument/basic_argument.h"

struct WorkloadArgumentContainer : ArgumentContainer {
    PositiveIntegerArgument iterations;
    BooleanArgument synchronize;

    WorkloadArgumentContainer()
        : iterations(*this, "iterations", "Number of iterations to perform"),
          synchronize(*this, "synchronize", "Wait for synchronization before each iteration") {}
};
