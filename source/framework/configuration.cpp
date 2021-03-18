#include "framework/configuration.h"

#include "framework/benchmark_info.h"

std::unique_ptr<Configuration> Configuration::instance = {};

Configuration::Configuration()
    : oclPlatformIndex(*this, "oclPlatformIndex", "OpenCL platform index"),
      oclDeviceIndex(*this, "oclDeviceIndex", "OpenCL device index inside the platform"),
      oclUseOOQ(*this, "oclUseOOQ", "Use out of order queue if it is supported"),
      l0DriverIndex(*this, "l0DriverIndex", "LevelZero driver index"),
      l0DeviceIndex(*this, "l0DeviceIndex", "LevelZero device index inside the driver"),
      subDeviceSelection(*this, "subDeviceSelection", "Device to be used in the benchmarks. Might be ignored by some specific tests"),
      csv(*this, "csv", "dump results in CSV format for easy imports to spreadsheets"),
      verbose(*this, "verbose", "dump results from all iterations"),
      iterations(*this, "iterations", "select how many times each test will be run"),
      selectedApi(*this, "api", "Compute API to be used"),
      noIntelExtensions(*this, "no-intel-extensions", "do not run benchmark requiring Intel specific extensions"),
      dumpCommandLines(*this, "dumpCommandLines", "output commandline arguments to run the each test"),
      noop(*this, "noop", "do not run any tests, only print their names and parameters"),
      noHeaders(*this, "noHeaders", "Do not print any informational messages at the top of the output"),
      dumpErrorsImmediately(*this, "dumpErrorsImmediately", "print errors to stdout immediately after they happen, not at the end of the run"),
      argFilter(*this, "argFilter", "filter tests by their arguments"),
      testFilter(*this, "testFilter", "filter tests by their names"),
      benchmarkSpecificConfiguration(createBenchmarkSpecificConfiguration()) {

    // OCL params
    oclPlatformIndex = 0;
    oclDeviceIndex = 0;
    oclUseOOQ = true;

    // L0 params
    l0DriverIndex = 0;
    l0DeviceIndex = 0;

    // Api agnostic params
    csv = false;
    verbose = false;
    iterations = 10;
    selectedApi = Api::All;
    noIntelExtensions = false;
    dumpCommandLines = false;
    subDeviceSelection = DeviceSelection::Root;
    noop = false;
    noHeaders = false;
    dumpErrorsImmediately = false;
    argFilter = std::vector<std::string>();
    testFilter = std::vector<std::string>();
}

Configuration::~Configuration() {
    delete benchmarkSpecificConfiguration;
}

bool Configuration::parseArgumentsForConfiguration(CommandLineArguments &arguments) {
    loadDefaultConfiguration();
    auto configuration = Configuration::instance.get();

    for (auto &argument : arguments) {
        if (!configuration->parseArgument(argument)) {
            return false;
        }
    }

    if (!configuration->validateArguments()) {
        return false;
    }

    if (configuration->csv) {
        configuration->printType = Configuration::PrintType::Csv;
    }
    if (configuration->verbose) {
        configuration->printType = Configuration::PrintType::DefaultWithVerbose;
    }

    return true;
}

void Configuration::loadDefaultConfiguration() {
    FATAL_ERROR_IF(Configuration::instance != nullptr, "Configuration parsed multiple times");
    Configuration::instance = std::make_unique<Configuration>();
}

Configuration &Configuration::get() {
    FATAL_ERROR_IF(Configuration::instance == nullptr, "Configuration was not parsed");
    return *Configuration::instance;
}

bool Configuration::validateArgumentsExtra() const {
    if (csv && verbose) {
        return false;
    }
    return true;
}

BenchmarkInfo::BenchmarkSpecificConfigurationBase *Configuration::createBenchmarkSpecificConfiguration() {
    if (BenchmarkInfo::isSet()) {
        return BenchmarkInfo::get().createBenchmarkSpecificConfiguration(*this).release();
    }
    return nullptr;
}
