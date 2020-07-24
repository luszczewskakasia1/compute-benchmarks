#pragma once

#include "framework/string_utils.h"

struct Configuration {
    enum class PrintType {
        Default,
        Csv,
    } printType = PrintType::Default;
    int iterations = 10;
};

extern Configuration configuration;
void parseArgumentsForConfiguration(int argc, char **argv);
