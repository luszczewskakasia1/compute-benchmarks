/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2026 Intel Corporation
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

#include "docs_generator/data_types.h"

#include <string>

std::vector<Api> getApisForBenchmark(const Benchmark &benchmark, const std::string &currentLocation) {
    std::vector<Api> apis = {Api::L0, Api::OpenCL};
    if (benchmark.locations.size() > 1) {
        for (const auto &possibleLocation : benchmark.locations) {
            if (possibleLocation == "embargo/cuda" && currentLocation != "") {
                apis.push_back(Api::OPT);
            }
        }
    }
    return apis;
}
