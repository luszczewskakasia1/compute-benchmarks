#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/timer.h"
#include "pci_benchmark/usm_copy.h"

#include <gtest/gtest.h>

static TestResult run(const UsmCopyArguments &arguments, Statistics &statistics) {
    // Setup
    Opencl opencl;
    Timer timer;
    auto clHostMemAllocINTEL = (pfn_clHostMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clHostMemAllocINTEL");
    auto clDeviceMemAllocINTEL = (pfn_clDeviceMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clDeviceMemAllocINTEL");
    auto clMemFreeINTEL = (pfn_clMemFreeINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clMemFreeINTEL");
    auto clEnqueueMemcpyINTEL = (pfn_clEnqueueMemcpyINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clEnqueueMemcpyINTEL");
    if (!clHostMemAllocINTEL || !clDeviceMemAllocINTEL || !clMemFreeINTEL || !clEnqueueMemcpyINTEL) {
        return TestResult::DriverFunctionNotFound;
    }
    cl_int retVal;

    // Create buffers
    void *source{}, *destination{};
    if (isDeviceMemory(arguments.transferDirection, TransferOperand::Source)) {
        source = clDeviceMemAllocINTEL(opencl.context, opencl.device, nullptr, arguments.size, 0u, &retVal);
    } else {
        source = clHostMemAllocINTEL(opencl.context, nullptr, arguments.size, 0u, &retVal);
    }
    if (isDeviceMemory(arguments.transferDirection, TransferOperand::Destination)) {
        destination = clDeviceMemAllocINTEL(opencl.context, opencl.device, nullptr, arguments.size, 0u, &retVal);
    } else {
        destination = clHostMemAllocINTEL(opencl.context, nullptr, arguments.size, 0u, &retVal);
    }

    // Warmup
    EXPECT_CL_SUCCESS(clEnqueueMemcpyINTEL(opencl.commandQueue, CL_FALSE, destination, source, arguments.size, 0, nullptr, nullptr));
    EXPECT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        EXPECT_CL_SUCCESS(clEnqueueMemcpyINTEL(opencl.commandQueue, CL_FALSE, destination, source, arguments.size, 0, nullptr, nullptr));
        timer.measureStart();
        EXPECT_CL_SUCCESS(clFinish(opencl.commandQueue));
        timer.measureEnd();
        statistics.pushValue(timer.getBandwidth(arguments.size));
    }
    return TestResult::Success;
}

static RegisterTestCase<UsmCopy> registerTestCase(run, Api::OpenCL);
