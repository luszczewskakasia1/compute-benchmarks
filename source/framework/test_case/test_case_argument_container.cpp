/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021 Intel Corporation
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit this
 * software or the related documents without Intel's prior written permission.
 *
 * This software and the related documents are provided as is, with no express or
 * implied warranties, other than those that are expressly stated in the License.
 */

#include "test_case_argument_container.h"

#include "framework/argument/abstract/argument.h"

#include <sstream>

std::string TestCaseArgumentContainer::getCurrentConfig(bool commandLine) const {
    std::ostringstream result;
    for (auto i = 0u; i < arguments.size(); i++) {
        if (commandLine) {
            result << "--";
        }
        result << arguments[i]->toString();
        if (i != arguments.size() - 1) {
            result << " ";
        }
    }
    return result.str();
}

bool TestCaseArgumentContainer::validateArguments() const {
    if (!ArgumentContainer::validateArguments()) {
        return false;
    }

    if (!validateApi(api)) {
        return false;
    }

    if (iterations <= 0) {
        return false;
    }

    return true;
}
