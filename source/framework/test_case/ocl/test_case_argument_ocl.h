#pragma once

#include "framework/ocl/opencl.h"
#include "framework/test_case/test_case_argument.h"

struct MapFlagsTestCaseArgument : TestCaseArgument {
    MapFlagsTestCaseArgument(TestCaseArguments &parent, const std::string &key)
        : TestCaseArgument(parent, key, "(read, write or writeInvalidate)") {}

    operator cl_map_flags() const {
        return value;
    }

    MapFlagsTestCaseArgument &operator=(cl_map_flags value) {
        this->value = value;
        return *this;
    }

    bool validate() const {
        switch (value) {
        case CL_MAP_READ:
        case CL_MAP_WRITE:
        case CL_MAP_WRITE_INVALIDATE_REGION:
            return true;
        default:
            return false;
        }
    }

  protected:
    std::string toStringValue() const override {
        switch (value) {
        case CL_MAP_READ:
            return "read";
        case CL_MAP_WRITE:
            return "write";
        case CL_MAP_WRITE_INVALIDATE_REGION:
            return "writeInvalidate";
        default:
            ERROR("Unknown map flag");
        }
    }

    void parseImpl(const std::string &value) override {
        const std::string valueLower = toLower(value);
        if (valueLower == "read") {
            this->value = CL_MAP_READ;
        } else if (valueLower == "write") {
            this->value = CL_MAP_WRITE;
        } else if (valueLower == "writeinvalidate") {
            this->value = CL_MAP_WRITE_INVALIDATE_REGION;
        } else {
            this->value = 0;
        }
    }

    cl_map_flags value = 0;
};
