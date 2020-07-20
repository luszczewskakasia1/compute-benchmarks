#pragma once

#include "framework/opencl.h"
#include "framework/test_case.h"
#include "framework/timer.h"

#include <gtest/gtest.h>

namespace UllsTest {

struct EmptyKernelArguments : TestCaseArguments {
    size_t workgroupCount;
    size_t workgroupSize;

    std::string getHelp() override {
        return "\t--workgroupCount=X\n"
               "\t--workgroupSize=X\n";
    }

    std::string getCurrentConfig() override {
        const auto gws = workgroupCount * workgroupSize;
        const auto lws = workgroupSize;
        std::ostringstream result;
        result << "gws=" << gws << ",lws=" << lws << "";
        return result.str();
    }

    bool parseArgument(const std::string &key, const std::string &value) override {
        if (key == "--workgroupCount") {
            workgroupCount = std::atoi(value.c_str());
        }
        if (key == "--workgroupSize") {
            workgroupSize = std::atoi(value.c_str());
        }
        return true;
    }

    bool validateArguments() override {
        return workgroupCount != 0 && workgroupSize != 0;
    }
};

class EmptyKernel : public TestCase<EmptyKernelArguments> {
  public:
    using TestCase<EmptyKernelArguments>::TestCase;

    std::string getHelp() override {
        return "enqueues empty kernel to measure walker spawn time.";
    }

    std::string getTestCaseName() override {
        return "EmptyKernel";
    }

    bool runOcl(const EmptyKernelArguments &arguments, Statistics &statistics) override {
        // Setup
        Opencl opencl;
        Timer timer;
        cl_int retVal;
        const auto gws = arguments.workgroupCount * arguments.workgroupSize;
        const auto lws = arguments.workgroupSize;

        // Create kernel
        const char *source = "__kernel void empty() {}";
        const auto sourceLength = strlen(source);
        cl_program program = clCreateProgramWithSource(opencl.context, 1, &source, &sourceLength, &retVal);
        ASSERT_CL_SUCCESS(retVal);
        retVal = clBuildProgram(program, 1, &opencl.device, nullptr, nullptr, nullptr);
        ASSERT_CL_SUCCESS(retVal);
        cl_kernel kernel = clCreateKernel(program, "empty", &retVal);
        ASSERT_CL_SUCCESS(retVal);

        // Warmup, kernel
        size_t warmupOffset = 8;
        retVal |= clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, &warmupOffset, &gws, &lws, 0, nullptr, nullptr);
        retVal |= clFinish(opencl.commandQueue);
        ASSERT_CL_SUCCESS(retVal);

        // Benchmark
        for (int i = 0; i < arguments.iterations; i++) {
            // Enqueue empty kernel and measure it
            timer.measureStart();
            retVal |= clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, &lws, 0, nullptr, nullptr);
            retVal |= clFinish(opencl.commandQueue);
            timer.messureEnd();
            ASSERT_CL_SUCCESS(retVal);
            statistics.pushValue(timer.Get());
        }

        // Cleanup
        ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
        ASSERT_CL_SUCCESS(clReleaseProgram(program));
        return true;
    }
};

} // namespace UllsTest
