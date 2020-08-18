#pragma once

#include "framework/string_utils.h"
#include "framework/api.h"

struct Configuration {
    enum class PrintType {
        Default,
        Csv,
        Verbose,
    } printType = PrintType::Default;
    int iterations = 10;
    Api selectedApi = Api::All;
    bool noIntelExtensions = false;
};

extern Configuration configuration;
bool parseArgumentsForConfiguration(int argc, char **argv);
