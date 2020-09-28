#pragma once

#include "framework/ocl/opencl.h"

namespace ProfilingHelper {

inline cl_int getEventDurationInNanoseconds(cl_event &profilingEvent, cl_ulong &timeNs) {
    cl_ulong start{};
    cl_int retVal = clGetEventProfilingInfo(profilingEvent, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, nullptr);
    if (retVal != CL_SUCCESS) {
        return retVal;
    }

    cl_ulong end{};
    retVal = clGetEventProfilingInfo(profilingEvent, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, nullptr);
    if (retVal != CL_SUCCESS) {
        return retVal;
    }

    timeNs = end - start;
    return CL_SUCCESS;
}

} // namespace ProfilingHelper
