#pragma once

#include "framework/benchmark_info.h"

struct BenchmarkSpecificConfiguration : BenchmarkSpecificConfigurationBase {
    bool printBandwidth = true;

    bool parseArgument(const std::string &key, const std::string &value) {
        if (key == "printBandwidth") {
            this->printBandwidth = std::atoi(value.c_str());
        }
        return true;
    }
};
