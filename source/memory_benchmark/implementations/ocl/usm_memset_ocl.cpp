#include "framework/ocl/opencl.h"
#include "framework/ocl/usm_helper.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"
#include "memory_benchmark/definitions/usm_memset.h"

#include <gtest/gtest.h>

static TestResult run(const UsmMemsetArguments &arguments, Statistics &statistics) {
    // Setup
    QueueProperties queueProperties = QueueProperties::create().setForceBlitter(arguments.forceBlitter).allowCreationFail();
    Opencl opencl(queueProperties);
    if (opencl.commandQueue == nullptr) {
        return TestResult::DeviceNotCapable;
    }
    Timer timer;
    auto clMemFreeINTEL = (pfn_clMemFreeINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clMemFreeINTEL");
    auto clEnqueueMemsetINTEL = (pfn_clEnqueueMemsetINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clEnqueueMemsetINTEL");
    if (!UsmHelper::supportsUsm(opencl.platform)) {
        return TestResult::DriverFunctionNotFound;
    }
    cl_int retVal;

    // Create buffer
    void *buffer = UsmHelper::allocate(arguments.memoryPlacement, opencl.platform, opencl.context, opencl.device, arguments.bufferSize, &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Warmup
    const uint8_t memsetValue = 0x1;
    ASSERT_CL_SUCCESS(clEnqueueMemsetINTEL(opencl.commandQueue, buffer, memsetValue, arguments.bufferSize, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        timer.measureStart();
        ASSERT_CL_SUCCESS(clEnqueueMemsetINTEL(opencl.commandQueue, buffer, memsetValue, arguments.bufferSize, 0, nullptr, nullptr));
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));
        timer.measureEnd();
        statistics.pushValue(timer.get(), arguments.bufferSize);
    }

    ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, buffer));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<UsmMemset> registerTestCase(run, Api::OpenCL, true);
