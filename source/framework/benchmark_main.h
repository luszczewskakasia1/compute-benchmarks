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

#include "framework/utility/command_line_argument.h"

#include <string>

class BenchmarkMain {
  public:
    BenchmarkMain(int argc, char **argv, const std::string benchmarkVersion);
    int main();

  private:
    int argc;
    char **argv;
    const std::string benchmarkVersion;
    CommandLineArguments commandLineArguments = {};

    int setupEnvironment();

    int printVersion(bool enableWarning, const char *prefix = "");
    void printHelp();
    int generateDocs(const std::string &docsFileName);

    int executeSingleTest(const std::string &testName, CommandLineArguments &commandLineArguments);
    int executeAllTests(CommandLineArguments &commandLineArguments);
};
