#pragma once

#include "framework/opencl.h"
#include "framework/test_case.h"
#include "framework/timer.h"

#include <gtest/gtest.h>

struct UllsBestSubmissionWithWalkerArguments : TestCaseArguments {};

class UllsBestSubmissionWithWalker : public TestCase<UllsBestSubmissionWithWalkerArguments> {
  public:
    using TestCase<UllsBestSubmissionWithWalkerArguments>::TestCase;

    std::string getHelp() override {
        return "UllsBestSubmissionWithWalker - enqueues kernel which updates system memory location and waits for the update in busy loop\n";
    }

    void runOcl(const UllsBestSubmissionWithWalkerArguments &arguments, int iterations) override {
        // Setup
        Opencl opencl;
        Timer timer;
        auto clHostMemAllocINTEL = (pfn_clHostMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clHostMemAllocINTEL");
        auto clMemFreeINTEL = (pfn_clMemFreeINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clMemFreeINTEL");
        cl_int retVal;

        // Create system memory buffer
        int *hostMemory = (int *)clHostMemAllocINTEL(opencl.context, nullptr, 64, 0, &retVal);
        ASSERT_EQ(CL_SUCCESS, retVal);

        // Create kernel
        const char *source = "__kernel void write(__global int *outBuffer) {  \n"
                             "   outBuffer[get_global_id(0)] = 1;             \n"
                             "}";
        const auto sourceLength = strlen(source);
        cl_program program = clCreateProgramWithSource(opencl.context, 1, &source, &sourceLength, &retVal);
        ASSERT_EQ(CL_SUCCESS, retVal);
        retVal = clBuildProgram(program, 1, &opencl.device, nullptr, nullptr, nullptr);
        ASSERT_EQ(CL_SUCCESS, retVal);
        cl_kernel kernel = clCreateKernel(program, "write", &retVal);
        ASSERT_EQ(CL_SUCCESS, retVal);

        // Benchmark
        ASSERT_EQ(CL_SUCCESS, retVal);
        retVal = clSetKernelArgSVMPointer(kernel, 0, hostMemory);
        size_t gws = 1;

        for (int i = 0; i < iterations; i++) {
            // Reset value
            *hostMemory = 0;

            // Warmup, kernel
            size_t warmupOffset = 8;
            clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, &warmupOffset, &gws, nullptr, 0, nullptr, nullptr);
            clFinish(opencl.commandQueue);
            ASSERT_EQ(CL_SUCCESS, retVal);

            // Enqueue write on GPU and poll for update on CPU
            timer.measureStart();
            clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, nullptr, 0, nullptr, nullptr);
            clFlush(opencl.commandQueue);
            ASSERT_EQ(CL_SUCCESS, retVal);

            while (*hostMemory != 1) {
            }
            timer.messureEnd();

            std::cout << timer.Get() << "   ";
        }
        std::cout << std::endl;

        // Cleanup
        ASSERT_EQ(CL_SUCCESS, clMemFreeINTEL(opencl.context, hostMemory));
        ASSERT_EQ(CL_SUCCESS, clReleaseKernel(kernel));
        ASSERT_EQ(CL_SUCCESS, clReleaseProgram(program));
    }
};
