#pragma once

#include "framework/enum/work_item_id_usage.h"
#include "framework/test_case_argument/abstract/test_case_argument_enum.h"

struct WorkItemIdUsageTestCaseArgument : EnumTestCaseArgument<WorkItemIdUsageTestCaseArgument, WorkItemIdUsage> {
    using EnumTestCaseArgument::EnumTestCaseArgument;
    ThisType &operator=(EnumType value) {
        this->value = value;
        return *this;
    }

    const static inline std::string enumName = "work item id usage";
    const static inline EnumType invalidEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[3] = {EnumType::None, EnumType::Global, EnumType::Local};
    const static inline std::string enumValuesNames[3] = {"None", "Global", "Local"};
};
