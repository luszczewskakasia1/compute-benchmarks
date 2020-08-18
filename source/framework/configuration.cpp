#include "framework/configuration.h"

Configuration configuration;

#define FAIL_IF_VALUE_WAS_PASSED \
    if (value != "") {           \
        return false;            \
    }

bool parseArgumentsForConfiguration(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        const auto argument = std::string{argv[i]};
        std::string key, value;
        if (!parseArgumentToKeyValue(argument, key, value)) {
            return false;
        }

        if (key == "iterations") {
            ::configuration.iterations = std::atoi(value.c_str());
            if (::configuration.iterations == 0) {
                return false;
            }
        }
        if (key == "csv") {
            FAIL_IF_VALUE_WAS_PASSED
            ::configuration.printType = Configuration::PrintType::Csv;
        }
        if (key == "verbose") {
            FAIL_IF_VALUE_WAS_PASSED
            ::configuration.printType = Configuration::PrintType::Verbose;
        }
        if (key == "no-intel-extensions") {
            FAIL_IF_VALUE_WAS_PASSED
            ::configuration.noIntelExtensions = true;
        }
        if (key == "api") {
            ::configuration.selectedApi = parseApi(value);
            if (::configuration.selectedApi == Api::Unknown) {
                return false;
            }
        }
    }
    return true;
}
