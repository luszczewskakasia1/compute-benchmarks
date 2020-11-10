#include "framework/configuration.h"

#include "framework/benchmark_info.h"

Configuration configuration;

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
      argFilter(*this, "argFilter", "filter tests by their arguments"),
      testFilter(*this, "testFilter", "filter tests by their names"),
      benchmarkSpecificConfiguration(BenchmarkSpecificConfigurationBase::create(*this).release()) {

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
    argFilter = std::vector<std::string>();
    testFilter = std::vector<std::string>();
}

Configuration::~Configuration() {
    delete benchmarkSpecificConfiguration;
}

bool parseArgumentsForConfiguration(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        const auto argument = std::string{argv[i]};
        std::string key, value;
        if (!parseArgumentToKeyValue(argument, key, value)) {
            return false;
        }

        if (!::configuration.parseArgument(key, value)) {
            return false;
        }
    }

    if (!::configuration.validateArguments()) {
        return false;
    }

    if (::configuration.csv) {
        ::configuration.printType = Configuration::PrintType::Csv;
    }
    if (::configuration.verbose) {
        ::configuration.printType = Configuration::PrintType::Verbose;
    }

    return true;
}

bool Configuration::validateArgumentsExtra() const {
    if (csv && verbose) {
        return false;
    }
    return true;
}
