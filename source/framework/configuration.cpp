#include "framework/configuration.h"

#include "framework/benchmark_info.h"

Configuration configuration;

Configuration::Configuration()
    : oclPlatformIndex(arguments, "oclPlatformIndex"),
      oclDeviceIndex(arguments, "oclDeviceIndex"),
      oclUseOOQ(arguments, "oclUseOOQ"),
      l0DriverIndex(arguments, "l0DriverIndex"),
      l0DeviceIndex(arguments, "l0DeviceIndex"),
      csv(arguments, "csv"),
      verbose(arguments, "verbose"),
      iterations(arguments, "iterations"),
      selectedApi(arguments, "api"),
      noIntelExtensions(arguments, "no-intel-extensions") {

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
}

Configuration::~Configuration() {
    delete benchmarkSpecificConfiguration;
}

bool parseArgumentsForConfiguration(int argc, char **argv) {
    ::configuration.benchmarkSpecificConfiguration = BenchmarkSpecificConfigurationBase::create().release();
    const auto benchmarkSpecificConfiguration = ::configuration.benchmarkSpecificConfiguration;

    for (int i = 1; i < argc; i++) {
        const auto argument = std::string{argv[i]};
        std::string key, value;
        if (!parseArgumentToKeyValue(argument, key, value)) {
            return false;
        }

        if (!::configuration.arguments.parseArgument(key, value)) {
            return false;
        }

        if (benchmarkSpecificConfiguration && !benchmarkSpecificConfiguration->parseArgument(key, value)) {
            return false;
        }
    }

    if (::configuration.csv && ::configuration.verbose) {
        return false;
    }
    if (::configuration.csv) {
        ::configuration.printType = Configuration::PrintType::Csv;
    }
    if (::configuration.verbose) {
        ::configuration.printType = Configuration::PrintType::Verbose;
    }

    if (!::configuration.arguments.validateArguments()) {
        return false;
    }

    return true;
}
