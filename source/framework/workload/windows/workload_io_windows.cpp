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

#include "framework/workload/workload_io.h"

#include <iostream>

class WorkloadIoWindows : public WorkloadIo {
  public:
    void writeToConsole(const std::string &message) override {
        std::cerr << message;
    }

    void writeToMeasurements(const std::string &measurements) override {
        std::cout << measurements << ' ';
    }

    void writeSynchronizationChar(char c) override {
        std::cout << c;
    }

    char readSynchronizationChar() override {
        return static_cast<char>(std::cin.get());
    }
};

std::unique_ptr<WorkloadIo> WorkloadIo::create(const WorkloadArgumentContainer &arguments) {
    return std::unique_ptr<WorkloadIo>(new WorkloadIoWindows());
}
