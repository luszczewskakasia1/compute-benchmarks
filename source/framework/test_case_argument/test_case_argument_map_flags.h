#pragma once

#include "framework/enum/map_flags.h"
#include "framework/test_case_argument/test_case_argument.h"
#include "framework/utility/string_utils.h"

struct MapFlagsTestCaseArgument : TestCaseArgument {
    MapFlagsTestCaseArgument(TestCaseArguments &parent, const std::string &key)
        : TestCaseArgument(parent, key, "(read, write or writeInvalidate)") {}

    operator MapFlags() const {
        return value;
    }

    MapFlagsTestCaseArgument &operator=(MapFlags value) {
        this->value = value;
        return *this;
    }

    bool validate() const {
        switch (value) {
        case MapFlags::Read:
        case MapFlags::Write:
        case MapFlags::WriteInvalidate:
            return true;
        default:
            return false;
        }
    }

  protected:
    std::string toStringValue() const override {
        switch (value) {
        case MapFlags::Read:
            return "read";
        case MapFlags::Write:
            return "write";
        case MapFlags::WriteInvalidate:
            return "writeInvalidate";
        default:
            ERROR("Unknown map flag");
        }
    }

    void parseImpl(const std::string &value) override {
        const std::string valueLower = toLower(value);
        if (valueLower == "read") {
            this->value = MapFlags::Read;
        } else if (valueLower == "write") {
            this->value = MapFlags::Write;
        } else if (valueLower == "writeinvalidate") {
            this->value = MapFlags::WriteInvalidate;
        } else {
            this->value = MapFlags::Unknown;
        }
    }

    MapFlags value = MapFlags::Unknown;
};
