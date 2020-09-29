#pragma once

#include "framework/configuration.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

#include <iomanip>
#include <vector>

struct LongHexTestCaseArgument : TestCaseArgument {
    using TestCaseArgument::TestCaseArgument;

    operator const std::vector<uint8_t> &() const {
        return bytes;
    }

    LongHexTestCaseArgument &operator=(const std::string &hex) {
        parseImpl(hex);
        return *this;
    }

    bool validate() const override {
        return bytes.size() > 0;
    }

  protected:
    std::string toStringValue() const override {
        std::ostringstream output{};
        output << "0x";
        for (auto byte : bytes) {
            output << std::uppercase << std::hex                      // print uppercase hex letters
                   << std::setfill('0') << std::setw(2) << std::right // always print 2 signs: 0x01 instead of 0x1
                   << static_cast<uint32_t>(byte);                    // cast to bigger type so it's not treated as char
        }
        return output.str();
    }

    void parseImpl(const std::string &value) override {
        bytes.resize(0);

        // Process prefix
        const std::string prefix = "0x";
        if (value.find(prefix) != 0) {
            return;
        }

        // Each byte is two characters, so length should be even
        if (value.length() % 2 != 0) {
            return;
        }

        // Iterate over bytes
        const auto bytesCount = value.length() / 2 - 1;
        bytes.reserve(bytesCount);
        for (auto byteIndex = 0u; byteIndex < bytesCount; byteIndex++) {
            const auto byteStringOffset = 2 + 2 * byteIndex;
            const auto byteString = value.substr(byteStringOffset, 2);
            const auto byte = static_cast<uint8_t>(std::stoul(byteString, nullptr, 16));
            bytes.push_back(byte);
        }
    }

    std::vector<uint8_t> bytes;
};
