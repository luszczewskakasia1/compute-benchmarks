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
#include <chrono>

class Timer {
  public:
    using Clock = std::chrono::high_resolution_clock;

    void measureStart() {
        startTime = Clock::now();
    }

    void measureEnd() {
        endTime = Clock::now();
    }

    Clock::duration get() const {
        return endTime - startTime;
    }

  private:
    Clock::time_point startTime;
    Clock::time_point endTime;
};
