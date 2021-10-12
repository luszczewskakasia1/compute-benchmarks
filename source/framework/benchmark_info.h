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

#include "framework/enum/measurement_unit.h"

#include <memory>
#include <string>

struct ArgumentContainer;

// This class represent information specific to a specific benchmark, e.g. ulls_benchmark or memory_benchmark.
// It is used to configure the behaviour of some framework classes.
class BenchmarkInfo {
  private:
  public:
    // Singleton accessors
    static BenchmarkInfo &get();
    static void initialize(const std::string &name,
                           const std::string &description,
                           int testCaseColumnWidth);

    // Getters
    std::string getBenchmarkName() const;
    std::string getBenchmarkFilename() const;
    std::string getBenchmarkDescription() const;
    int getTestCaseNameColumnWidth() const;

  private:
    static std::unique_ptr<BenchmarkInfo> instance;
    std::string name;
    std::string description;
    int testCaseColumnWidth;
};
