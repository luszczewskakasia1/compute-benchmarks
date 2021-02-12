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

struct Configuration : ArgumentsBase {
  private:
    static std::unique_ptr<Configuration> instance;

  public:
    Configuration();
    ~Configuration();

    enum class PrintType {
        Default,
        DefaultWithVerbose,
        Csv,
    } printType = PrintType::Default;

    static bool parseArgumentsForConfiguration(CommandLineArguments &arguments);
    static void loadDefaultConfiguration();
    static Configuration &get();

    bool validateArgumentsExtra() const override;

    // OCL params
    NonNegativeIntegerArgument oclPlatformIndex;
    NonNegativeIntegerArgument oclDeviceIndex;
    BooleanArgument oclUseOOQ;

    // L0 params
    NonNegativeIntegerArgument l0DriverIndex;
    NonNegativeIntegerArgument l0DeviceIndex;

    // Api agnostic params
    DeviceSelectionArgument subDeviceSelection;
    BooleanFlagArgument csv;
    BooleanFlagArgument verbose;
    PositiveIntegerArgument iterations;
    ApiArgument selectedApi;
    BooleanFlagArgument noIntelExtensions;
    BooleanFlagArgument dumpCommandLines;
    BooleanFlagArgument noop;
    BooleanArgument dumpErrorsImmediately;
    StringListArgument argFilter;
    StringListArgument testFilter;

    BenchmarkInfo::BenchmarkSpecificConfigurationBase *benchmarkSpecificConfiguration = nullptr;

  private:
    BenchmarkInfo::BenchmarkSpecificConfigurationBase *createBenchmarkSpecificConfiguration();
};
