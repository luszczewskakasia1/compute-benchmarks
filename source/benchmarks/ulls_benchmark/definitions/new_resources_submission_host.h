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

#include "framework/argument/basic_argument.h"
#include "framework/test_case/test_case.h"

struct NewResourcesSubmissionHostArguments : TestCaseArgumentContainer {
    ByteSizeArgument size;

    NewResourcesSubmissionHostArguments()
        : size(*this, "size", "Size of the buffer") {}
};

struct NewResourcesSubmissionHost : TestCase<NewResourcesSubmissionHostArguments> {
    using TestCase<NewResourcesSubmissionHostArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "NewResourcesSubmissionHost";
    }

    std::string getHelp() const override {
        return "enqueues kernel that uses a buffer placed in host memory to measure resource "
               "preparation time. The resource is destroyed and recreated for each iteration "
               "to ensure it is a different memory allocation.";
    };
};
