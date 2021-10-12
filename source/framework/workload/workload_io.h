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

#include "framework/workload/workload_argument_container.h"

#include <memory>
#include <string>

class WorkloadIo {
  public:
    // Os-specific factory function
    static std::unique_ptr<WorkloadIo> create(const WorkloadArgumentContainer &arguments);

    virtual void writeToConsole(const std::string &message) = 0;
    virtual void writeToMeasurements(const std::string &measurements) = 0;
    virtual void writeSynchronizationChar(char c) = 0;
    virtual char readSynchronizationChar() = 0;
};
