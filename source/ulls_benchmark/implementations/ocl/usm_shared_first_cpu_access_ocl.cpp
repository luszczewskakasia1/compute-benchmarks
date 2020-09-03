#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/timer.h"
#include "ulls_benchmark/definitions/usm_shared_first_cpu_access.h"

#include <gtest/gtest.h>

static TestResult run(const UsmSharedFirstCpuAccessArguments &arguments, Statistics &statistics) {
    // Setup
    Opencl opencl;
    Timer timer;
    auto clSharedMemAllocINTEL = (pfn_clSharedMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clSharedMemAllocINTEL");
    auto clMemFreeINTEL = (pfn_clMemFreeINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clMemFreeINTEL");
    if (!clSharedMemAllocINTEL || !clMemFreeINTEL) {
        return TestResult::DriverFunctionNotFound;
    }
    cl_int retVal{};

    // Warmup
    auto buffer = clSharedMemAllocINTEL(opencl.context, opencl.device, nullptr, arguments.bufferSize, 0u, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    static_cast<uint8_t *>(buffer)[0] = 0;
    ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, buffer));

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        auto buffer = clSharedMemAllocINTEL(opencl.context, opencl.device, nullptr, arguments.bufferSize, 0u, &retVal);
        ASSERT_CL_SUCCESS(retVal);

        timer.measureStart();
        static_cast<uint32_t *>(buffer)[0] = 0;
        timer.measureEnd();

        statistics.pushValue(timer.Get());
        ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, buffer));
    }

    return TestResult::Success;
}

static RegisterTestCase<UsmSharedFirstCpuAccess> registerTestCase(run, Api::OpenCL, true);
