#pragma once

#include "framework/api.h"
#include "framework/string_utils.h"

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

    int iterations = 10;
    Api selectedApi = Api::All;
    bool noIntelExtensions = false;
};

extern Configuration configuration;
bool parseArgumentsForConfiguration(int argc, char **argv);
