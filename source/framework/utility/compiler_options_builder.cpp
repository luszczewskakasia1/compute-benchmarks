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

#include "compiler_options_builder.h"

#include "framework/utility/string_utils.h"

void CompilerOptionsBuilder::addOption(const char *option) {
    options << option << ' ';
}

void CompilerOptionsBuilder::addOptionOpenCl20() {
    addOption("-cl-std=CL2.0");
}

void CompilerOptionsBuilder::addDefinition(const char *key) {
    options << "-D" << key << ' ';
}

void CompilerOptionsBuilder::addDefinitionKeyValue(const char *key, const char *value) {
    options << "-D" << key << '=' << value << ' ';
}

void CompilerOptionsBuilder::addDefinitionKeyValue(const char *key, const std::string &value) {
    addDefinitionKeyValue(key, value.c_str());
}

void CompilerOptionsBuilder::addDefinitionKeyValue(const char *key, size_t value) {
    options << "-D" << key << '=' << value << ' ';
}

void CompilerOptionsBuilder::addMacro(const char *name, const std::vector<const char *> &arguments, const char *body) {
    const static auto toString = +[](const char *a) { return std::string(a); };
    options << "-D" << name
            << '(' << joinStrings(",", arguments, toString) << ')'
            << '='
            << body << ' ';
}

std::string CompilerOptionsBuilder::str() const {
    return options.str();
}
