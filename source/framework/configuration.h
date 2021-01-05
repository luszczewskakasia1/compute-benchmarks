#pragma once

#include "framework/benchmark_info.h"
#include "framework/test_case_argument/enum/test_case_argument_api.h"
#include "framework/test_case_argument/enum/test_case_argument_device_selection.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/test_case_argument/test_case_argument_boolean_flag.h"
#include "framework/test_case_argument/test_case_argument_string_list.h"
#include "framework/test_case_argument/test_case_arguments.h"
#include "framework/utility/command_line_argument.h"

#include <memory>

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
    DeviceSelectionTestCaseArgument subDeviceSelection;
    BooleanFlagTestCaseArgument csv;
    BooleanFlagTestCaseArgument verbose;
    PositiveIntegerTestCaseArgument iterations;
    ApiTestCaseArgument selectedApi;
    BooleanFlagTestCaseArgument noIntelExtensions;
    BooleanFlagTestCaseArgument dumpCommandLines;
    BooleanFlagTestCaseArgument noop;
    BooleanTestCaseArgument dumpErrorsImmediately;
    StringListTestCaseArgument argFilter;
    StringListTestCaseArgument testFilter;

    BenchmarkInfo::BenchmarkSpecificConfigurationBase *benchmarkSpecificConfiguration = nullptr;
};

extern Configuration configuration;
bool parseArgumentsForConfiguration(CommandLineArguments &arguments);
