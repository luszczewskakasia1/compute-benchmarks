#pragma once

#include "framework/string_utils.h"
#include "framework/test_case_argument/test_case_argument.h"
#include "framework/enum/transfer_direction.h"

struct TransferDirectionTestCaseArgument : TestCaseArgument {
    TransferDirectionTestCaseArgument(TestCaseArguments &parent, const std::string &key)
        : TestCaseArgument(parent, key, "(SysToSys, SysToDev, DevToSys or DevToDev)") {}

    operator TransferDirection() const {
        return value;
    }

    TransferDirectionTestCaseArgument &operator=(TransferDirection value) {
        this->value = value;
        return *this;
    }

    bool validate() const override {
        return this->value != TransferDirection::Unknown;
    }

  protected:
    std::string toStringValue() const override {
        switch (value) {
        case TransferDirection::SysToSys:
            return "SysToSys";
        case TransferDirection::SysToDev:
            return "SysToDev";
        case TransferDirection::DevToSys:
            return "DevToSys";
        case TransferDirection::DevToDev:
            return "DevToDev";
        default:
            ERROR("Unknown transfer direction");
        }
    }

    void parseImpl(const std::string &value) override {
        const std::string valueLower = toLower(value);
        if (valueLower == "systosys") {
            this->value = TransferDirection::SysToSys;
        } else if (valueLower == "systodev") {
            this->value = TransferDirection::SysToDev;
        } else if (valueLower == "devtosys") {
            this->value = TransferDirection::DevToSys;
        } else if (valueLower == "devtodev") {
            this->value = TransferDirection::DevToDev;
        } else {
            this->value = TransferDirection::Unknown;
        }
    }

    TransferDirection value = TransferDirection::Unknown;
};
