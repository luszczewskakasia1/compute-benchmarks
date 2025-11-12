/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2025 Intel Corporation
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

#include "framework/enum/api_additional.h"

#include "framework/enum/api.h"
#include "framework/utility/error.h"

#include <string>

std::string to_string_additional(Api api) {
    switch (api) {
    case Api::OPT:
        return "cuda";
    default:
        FATAL_ERROR("Unknown API");
    }
}

std::string getUserFriendlyAdditionalApiName(Api api) {
    switch (api) {
    case Api::OPT:
        return "CUDA";
    default:
        FATAL_ERROR("Unknown API");
    }
}

Api parseAdditionalApi(const std::string &value) {
    if (value == "cuda") {
        return Api::OPT;
    } else {
        return Api::Unknown;
    }
}
