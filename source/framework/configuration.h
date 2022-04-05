/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021-2022 Intel Corporation
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

#include "framework/argument/argument_container.h"
#include "framework/argument/basic_argument.h"
#include "framework/argument/boolean_flag_argument.h"
#include "framework/argument/enum/api_argument.h"
#include "framework/argument/enum/device_selection_argument.h"
#include "framework/argument/string_argument.h"
#include "framework/argument/string_list_argument.h"
#include "framework/utility/command_line_argument.h"

#include <memory>

struct Configuration : ArgumentContainer {
  private:
    static std::unique_ptr<Configuration> instance;

  public:
    Configuration();

    enum class PrintType {
        Default,
        DefaultWithVerbose,
        Csv,
    } printType = PrintType::Default;

    static bool parseArgumentsForConfiguration(CommandLineArguments &arguments);
    static void loadDefaultConfiguration();
    static Configuration &get();

    bool validateArgumentsExtra() const override;

    // Diagnostic params
    BooleanFlagArgument help;
    BooleanFlagArgument version;
    BooleanFlagArgument hwInfo;
    BooleanFlagArgument generateDocs;

    // OCL params
    IntegerArgument oclPlatformIndex;
    NonNegativeIntegerArgument oclDeviceIndex;
    BooleanArgument oclUseOOQ;

    // L0 params
    NonNegativeIntegerArgument l0DriverIndex;
    NonNegativeIntegerArgument l0DeviceIndex;

    // Api agnostic params
    StringArgument test;
    DeviceSelectionArgument subDeviceSelection;
    BooleanFlagArgument csv;
    BooleanFlagArgument verbose;
    BooleanFlagArgument interactivePrints;
    PositiveIntegerArgument iterations;
    ApiArgument selectedApi;
    BooleanFlagArgument noIntelExtensions;
    BooleanFlagArgument dumpCommandLines;
    BooleanFlagArgument noop;
    BooleanFlagArgument noHeaders;
    BooleanFlagArgument noColumnNames;
    BooleanFlagArgument doNotPrintBandwidth;
    BooleanArgument dumpErrorsImmediately;
    StringListArgument argFilter;
    StringListArgument testFilter;
    BooleanFlagArgument returnSubmissionTimeInsteadOfWorkloadTime;
    BooleanFlagArgument markTimers;
};
