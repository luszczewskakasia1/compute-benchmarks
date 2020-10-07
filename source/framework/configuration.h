#pragma once

#include "framework/enum/api.h"
#include "framework/test_case_argument/test_case_argument_api.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/test_case_argument/test_case_argument_boolean_flag.h"
#include "framework/test_case_argument/test_case_arguments.h"
#include "framework/utility/string_utils.h"

#include <memory>

struct BenchmarkSpecificConfigurationBase;

struct Configuration : TestCaseArgumentsBase {
    Configuration();
    ~Configuration();

    enum class PrintType {
        Default,
        Csv,
        Verbose,
    } printType = PrintType::Default;

    bool validateArgumentsExtra() const override;

    // OCL params
    NonNegativeIntegerTestCaseArgument oclPlatformIndex;
    NonNegativeIntegerTestCaseArgument oclDeviceIndex;
    BooleanTestCaseArgument oclUseOOQ;

    // L0 params
    NonNegativeIntegerTestCaseArgument l0DriverIndex;
    NonNegativeIntegerTestCaseArgument l0DeviceIndex;

    // Api agnostic params
    BooleanFlagTestCaseArgument csv;
    BooleanFlagTestCaseArgument verbose;
    PositiveIntegerTestCaseArgument iterations;
    ApiTestCaseArgument selectedApi;
    BooleanFlagTestCaseArgument noIntelExtensions;

    BenchmarkSpecificConfigurationBase *benchmarkSpecificConfiguration = nullptr;
};

extern Configuration configuration;
bool parseArgumentsForConfiguration(int argc, char **argv);
