#pragma once

#include "framework/enum/memory_placement.h"
#include "framework/string_utils.h"
#include "framework/test_case_argument/test_case_argument.h"

struct MemoryPlacementTestCaseArgument : TestCaseArgument {
    MemoryPlacementTestCaseArgument(TestCaseArguments &parent, const std::string &key)
        : TestCaseArgument(parent, key, "(host or device)") {}

    operator MemoryPlacement() const {
        return value;
    }

    MemoryPlacementTestCaseArgument &operator=(MemoryPlacement value) {
        this->value = value;
        return *this;
    }

    bool validate() const override {
        return this->value != MemoryPlacement::Unknown;
    }

  protected:
    std::string toStringValue() const override {
        switch (value) {
        case MemoryPlacement::Host:
            return "host";
        case MemoryPlacement::Device:
            return "device";
        default:
            ERROR("Unknown memory placement");
        }
    }

    void parseImpl(const std::string &value) override {
        const std::string valueLower = toLower(value);
        if (valueLower == "host") {
            this->value = MemoryPlacement::Host;
        } else if (valueLower == "device") {
            this->value = MemoryPlacement::Device;
        } else {
            this->value = MemoryPlacement::Unknown;
        }
    }

    MemoryPlacement value = MemoryPlacement::Unknown;
};
