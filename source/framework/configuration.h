#pragma once

#include "framework/argument/argument_container.h"
#include "framework/argument/basic_argument.h"
#include "framework/argument/boolean_flag_argument.h"
#include "framework/argument/enum/api_argument.h"
#include "framework/argument/enum/device_selection_argument.h"
#include "framework/argument/string_argument.h"
#include "framework/argument/string_list_argument.h"
#include "framework/benchmark_info.h"
#include "framework/utility/command_line_argument.h"

#include <memory>

struct Configuration : ArgumentContainer {
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

    // Diagnostic params
    BooleanFlagArgument help;
    BooleanFlagArgument version;
    BooleanFlagArgument hwInfo;
    BooleanFlagArgument generateDocs;
    StringArgument generateDocsPath;

    // OCL params
    IntegerArgument oclPlatformIndex;
    NonNegativeIntegerArgument oclDeviceIndex;
    BooleanArgument oclUseOOQ;

    // L0 params
    NonNegativeIntegerArgument l0DriverIndex;
    NonNegativeIntegerArgument l0DeviceIndex;

    // Api agnostic params
    StringArgument test;
    DeviceSelectionArgument subDeviceSelection;
    BooleanFlagArgument csv;
    BooleanFlagArgument verbose;
    BooleanFlagArgument interactivePrints;
    PositiveIntegerArgument iterations;
    ApiArgument selectedApi;
    BooleanFlagArgument noIntelExtensions;
    BooleanFlagArgument dumpCommandLines;
    BooleanFlagArgument noop;
    BooleanFlagArgument noHeaders;
    BooleanArgument dumpErrorsImmediately;
    StringListArgument argFilter;
    StringListArgument testFilter;
    BooleanFlagArgument returnSubmissionTimeInsteadOfWorkloadTime;

    BenchmarkInfo::BenchmarkSpecificConfigurationBase *benchmarkSpecificConfiguration = nullptr;

  private:
    BenchmarkInfo::BenchmarkSpecificConfigurationBase *createBenchmarkSpecificConfiguration();
};
