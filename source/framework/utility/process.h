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

#include "framework/test_case/test_result.h"

#include <string>
#include <vector>

class Process {
  public:
    Process(const std::string &exeName);
    Process(Process &&other);
    Process &operator=(Process &&other);
    ~Process();

    // Configuring process to be run
    void addArgument(const std::string &key, const std::string &value);
    void addEnvVariable(const std::string &key, const std::string &value);
    void addHandleForInheritance(int handle);
    void setName(const std::string &string) { this->processName = string; }

    // Getters
    std::vector<uint64_t> getMeasurements(size_t expectedCount);
    const std::string &getName() const { return this->processName; }

    // OS-specific methods
    void run();
    void waitForFinish();
    TestResult getResult();
    const std::string &getMeasurements();
    const std::string &getStdout();
    void synchronizationSignal();
    void synchronizationWait();

  private:
    void freeOsSpecificData();

    std::string exeName;
    std::vector<std::pair<std::string, std::string>> arguments;
    std::vector<std::pair<std::string, std::string>> envVariables;
    std::vector<int> handlesForInheritance;
    void *osSpecificData = nullptr;
    std::string processName = "";
};
