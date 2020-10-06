#pragma once

#include "framework/enum/api.h"
#include "framework/utility/string_utils.h"

#include <memory>

struct BenchmarkSpecificConfigurationBase;

struct Configuration {
    enum class PrintType {
        Default,
        Csv,
        Verbose,
    } printType = PrintType::Default;
    size_t oclPlatformIndex = 0;
    size_t oclDeviceIndex = 0;
    bool oclUseOOQ = true;
    size_t l0DriverIndex = 0;
    size_t l0DeviceIndex = 0;
    std::unique_ptr<BenchmarkSpecificConfigurationBase> benchmarkSpecificConfiguration = {};

    int iterations = 10;
    Api selectedApi = Api::All;
    bool noIntelExtensions = false;
};

extern Configuration configuration;
bool parseArgumentsForConfiguration(int argc, char **argv);
