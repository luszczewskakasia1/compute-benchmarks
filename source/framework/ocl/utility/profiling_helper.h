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

#include "framework/configuration.h"
#include "framework/ocl/opencl.h"

namespace ProfilingHelper {

inline cl_int getEventDurationInNanoseconds(cl_event &profilingEvent, cl_ulong &timeNs) {
    cl_ulong start{};
    auto firstTimestamp = CL_PROFILING_COMMAND_START;
    auto secondTimestamp = CL_PROFILING_COMMAND_END;

    if (Configuration::get().returnSubmissionTimeInsteadOfWorkloadTime) {
        firstTimestamp = CL_PROFILING_COMMAND_QUEUED;
        secondTimestamp = CL_PROFILING_COMMAND_START;
    }

    cl_int retVal = clGetEventProfilingInfo(profilingEvent, firstTimestamp, sizeof(cl_ulong), &start, nullptr);
    if (retVal != CL_SUCCESS) {
        return retVal;
    }

    cl_ulong end{};
    retVal = clGetEventProfilingInfo(profilingEvent, secondTimestamp, sizeof(cl_ulong), &end, nullptr);
    if (retVal != CL_SUCCESS) {
        return retVal;
    }

    timeNs = end - start;
    return CL_SUCCESS;
}

} // namespace ProfilingHelper
