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

#include "framework/utility/statistics.h"

#include <chrono>
#include <sstream>

class WorkloadIo;

class WorkloadStatistics : public Statistics {
  public:
    using Statistics::Statistics;
    using Clock = std::chrono::high_resolution_clock;

    void printStatistics(WorkloadIo &io);

    void pushValue(Clock::duration time, MeasurementUnit unit, MeasurementType type, const std::string &description = "") override;
    void pushValue(Clock::duration time, uint64_t size, MeasurementUnit unit, MeasurementType type, const std::string &description = "") override;

    bool isEmpty() const override;
    bool isFull() const override;

  private:
    std::ostringstream result{};
    size_t samplesCount = 0;
};
