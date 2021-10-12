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

#include "framework/utility/error.h"

#include <sstream>
#include <string>

struct ArgumentContainer;
class CommandLineArgument;

struct Argument {
    Argument(ArgumentContainer &parent, const std::string &key) : Argument(parent, key, "") {}
    Argument(ArgumentContainer &parent, const std::string &key, const std::string &extraHelp);

    const std::string getKey() const;
    std::string getHelp() const;
    std::string getExtraHelp() const;

    virtual std::string toString() const {
        std::ostringstream result;
        result << key << "=" << toStringValue();
        return result.str();
    }

    void parse(CommandLineArgument &argument);
    void markAsParsed();

    virtual bool validate() const {
        return true;
    }

    bool wasParsed() const {
        return parsed;
    }

  protected:
    virtual void parseImpl(const std::string &value) = 0;
    virtual std::string toStringValue() const = 0;
    virtual std::string getHelpEntry(const std::string &key) const;

  private:
    const std::string key;
    const std::string extraHelp;
    bool parsed = false;
};
