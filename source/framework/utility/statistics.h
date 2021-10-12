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
#include "framework/utility/error.h"

#include <chrono>

class Statistics {
  public:
    using Clock = std::chrono::high_resolution_clock;

    Statistics(size_t maxSamplesCount) : maxSamplesCount(maxSamplesCount) {}

    virtual void pushValue(Clock::duration time, MeasurementUnit unit, MeasurementType type, const std::string &description = "") = 0;
    virtual void pushValue(Clock::duration time, uint64_t size, MeasurementUnit unit, MeasurementType type, const std::string &description = "") = 0;

    virtual bool isEmpty() const = 0;
    virtual bool isFull() const = 0;

  protected:
    const size_t maxSamplesCount = 0;
};
