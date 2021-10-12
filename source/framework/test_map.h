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

#pragma once

#include "framework/test_case/test_case_interface.h"

#include <map>
#include <memory>
#include <string>

struct LexicographicalLess {
    bool operator()(const std::string &left, const std::string &right) const {
        return left < right;
    }
};

struct TestMap : std::map<std::string, std::unique_ptr<TestCaseInterface>, LexicographicalLess> {
    static TestMap &get() {
        static TestMap testMap = {};
        return testMap;
    }
};
