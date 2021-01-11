#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/test_case_argument/test_case_arguments.h"

struct WorkloadParameters : TestCaseArgumentsBase {
    PositiveIntegerTestCaseArgument iterations;

    WorkloadParameters()
        : iterations(*this, "iterations", "Number of iterations to perform") {}
};
