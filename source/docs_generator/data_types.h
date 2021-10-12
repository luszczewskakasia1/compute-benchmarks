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

#include "framework/enum/api.h"

#include <map>
#include <optional>
#include <unordered_set>

struct LexicographicalLess {
    bool operator()(const std::string &left, const std::string &right) const {
        return left < right;
    }
};

struct BenchmarkInstance {
    const std::string baseName;
    const std::string path;
    const Api api;

    BenchmarkInstance(const std::string &name, const std::string &path)
        : baseName(deduceBaseName(name)),
          path(path),
          api(deduceApi(name)) {}

    static std::string deduceBaseName(const std::string &name) {
        const auto position = name.find_last_of("_");
        FATAL_ERROR_IF(position == std::string::npos, "Could not deduce base name from ", name);
        return name.substr(0, position);
    }

    static Api deduceApi(const std::string &name) {
        const auto position = name.find_last_of("_");
        FATAL_ERROR_IF(position == std::string::npos, "Could not deduce API from ", name);
        const std::string apiString = name.substr(position + 1);
        const Api api = parseApi(apiString);
        FATAL_ERROR_IF(api == Api::Unknown || api == Api::All, "Could not parse API from ", name);
        return api;
    }
};

struct TestCaseArgument {
    std::string name;
    std::string help;
};
using TestCaseArguments = std::map<std::string, TestCaseArgument, LexicographicalLess>;

struct TestCase {
    std::string name = {};
    std::string help = {};
    std::unordered_set<Api> apis = {};
    TestCaseArguments arguments = {};
};
using TestCases = std::map<std::string, TestCase, LexicographicalLess>;

struct Benchmark {
    std::string baseName = {};
    std::vector<const BenchmarkInstance *> instances = {};
    std::string description = {};
    TestCases testCases;
};
using Benchmarks = std::map<std::string, Benchmark, LexicographicalLess>;
