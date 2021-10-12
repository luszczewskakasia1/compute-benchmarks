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

#include "framework/enum/measurement_type.h"
#include "framework/enum/measurement_unit.h"
#include "framework/utility/process.h"

#include <string>

class Statistics;

class ProcessGroup {
  public:
    ProcessGroup(const std::string &binaryName, size_t count);

    // Applying same operation for all processes
    void addArgumentAll(const std::string &key, const std::string &value);
    void addEnvVariableAll(const std::string &key, const std::string &value);
    void runAll();
    void synchronizeAll(size_t iterationsCount);
    void waitForFinishAll();
    TestResult getResultAll();

    // Operations involving all processes
    void pushMeasurementsToStatistics(size_t expectedCount,
                                      Statistics &statistics,
                                      MeasurementUnit unit,
                                      MeasurementType type,
                                      bool pushIndividualProcessesMeasurements,
                                      bool pushAveragedMeasurements);

    // Container-like methods
    Process &operator[](size_t index);
    size_t size() const;

  private:
    const std::string binaryName;
    std::vector<Process> processes = {};
};
