#pragma once

#include "framework/enum/work_item_id_usage.h"
#include "framework/string_utils.h"
#include "framework/test_case_argument/test_case_argument.h"

struct WorkItemIdUsageTestCaseArgument : TestCaseArgument {
    WorkItemIdUsageTestCaseArgument(TestCaseArguments &parent, const std::string &key)
        : TestCaseArgument(parent, key, "Specifies whether calls to get_global_id or get_local_id are made inside the kernel (none, global or local)") {}

    operator WorkItemIdUsage() const {
        return value;
    }

    WorkItemIdUsageTestCaseArgument &operator=(WorkItemIdUsage value) {
        this->value = value;
        return *this;
    }

    bool validate() const {
        switch (value) {
        case WorkItemIdUsage::None:
        case WorkItemIdUsage::Global:
        case WorkItemIdUsage::Local:
            return true;
        default:
            return false;
        }
    }

  protected:
    std::string toStringValue() const override {
        switch (value) {
        case WorkItemIdUsage::None:
            return "none";
        case WorkItemIdUsage::Global:
            return "global";
        case WorkItemIdUsage::Local:
            return "local";
        default:
            ERROR("Unknown work item id usage");
        }
    }

    void parseImpl(const std::string &value) override {
        const std::string valueLower = toLower(value);
        if (valueLower == "none") {
            this->value = WorkItemIdUsage::None;
        } else if (valueLower == "global") {
            this->value = WorkItemIdUsage::Global;
        } else if (valueLower == "local") {
            this->value = WorkItemIdUsage::Local;
        } else {
            this->value = WorkItemIdUsage::Unknown;
        }
    }

    WorkItemIdUsage value = WorkItemIdUsage::Unknown;
};
