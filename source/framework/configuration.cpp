#include "framework/configuration.h"

Configuration configuration;

void parseArgumentsForConfiguration(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        const auto argument = std::string{argv[i]};
        std::string key, value;
        if (!parseArgumentToKeyValue(argument, key, value)) {
            continue;
        }

        if (key == "--iterations") {
            ::configuration.iterations = std::atoi(value.c_str());
        }
        if (key == "--csv") {
            ::configuration.printType = Configuration::PrintType::Csv;
        }
        if (key == "--verbose") {
            ::configuration.printType = Configuration::PrintType::Verbose;
        }
    }
}
