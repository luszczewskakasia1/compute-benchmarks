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

#include "process.h"

#include "framework/utility/error.h"
#include "framework/utility/string_utils.h"

#include <iostream>

Process::Process(const std::string &exeName)
    : exeName(exeName) {
    arguments.emplace_back(exeName, std::string{});
}

Process::Process(Process &&other)
    : exeName(std::move(other.exeName)),
      arguments(std::move(other.arguments)),
      envVariables(std::move(other.envVariables)),
      osSpecificData(std::move(other.osSpecificData)) {
    other.osSpecificData = nullptr;
}

Process &Process::operator=(Process &&other) {
    exeName = std::move(other.exeName);
    arguments = std::move(other.arguments);
    envVariables = std::move(other.envVariables);
    osSpecificData = std::move(other.osSpecificData);
    other.osSpecificData = nullptr;
    return *this;
}

Process::~Process() {
    freeOsSpecificData();
}

void Process::addArgument(const std::string &key, const std::string &value) {
    arguments.emplace_back(std::string("--") + key, value);
}

void Process::addEnvVariable(const std::string &key, const std::string &value) {
    envVariables.emplace_back(key, value);
}

void Process::addHandleForInheritance(int handle) {
    handlesForInheritance.push_back(handle);
}

std::vector<uint64_t> Process::getMeasurements(size_t expectedCount) {
    const auto data = getMeasurements();
    const auto dataSplit = splitString(data);
    FATAL_ERROR_IF(dataSplit.size() != expectedCount, "Child process returned an invalid number of measurements");

    std::vector<uint64_t> measurementsFromProcess = {};
    for (const auto measurementString : dataSplit) {
        const auto measurement = std::atoll(measurementString.c_str());
        measurementsFromProcess.push_back(measurement);
    }

    return measurementsFromProcess;
}
