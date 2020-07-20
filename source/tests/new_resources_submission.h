#pragma once

#include "framework/opencl.h"
#include "framework/test_case.h"
#include "framework/timer.h"

#include <gtest/gtest.h>

namespace UllsTest {

struct NewResourcesSubmissionArguments : TestCaseArguments {
    size_t sizeMB;

    bool parseArgument(const std::string &key, const std::string &value) override {
        if (key == "--sizeMB") {
            sizeMB = std::atoi(value.c_str());
        }
        return true;
    }

    bool validateArguments() override {
        return sizeMB != 0;
    }
};

class NewResourcesSubmission : public TestCase<NewResourcesSubmissionArguments> {
  public:
    using TestCase<NewResourcesSubmissionArguments>::TestCase;

    std::string getHelp() override {
        return "enqueues kernel with a new resource to measure resource preparation time. Parameters:\n"
               "\t--sizeMB=X\n"
               "\n";
    };

    std::string getTestCaseName() override {
        return "NewResourcesSubmission";
    }

    std::string getTestCaseConfig(const NewResourcesSubmissionArguments &arguments) override {
        std::ostringstream result;
        result << "(sizeMB=" << arguments.sizeMB << ")";
        return result.str();
    }

    void runOcl(const NewResourcesSubmissionArguments &arguments, Statistics &statistics) override {
        // Setup
        Opencl opencl;
        Timer timer;
        cl_int retVal;

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

        // Warmup kernel
        const size_t gws = 1;
        const size_t sizeInBytes = arguments.sizeMB * 1024 * 1024;
        cl_mem buffer = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE, sizeInBytes, nullptr, &retVal);
        retVal |= clSetKernelArg(kernel, 0, sizeof(buffer), &buffer);
        retVal |= clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, nullptr, 0, nullptr, nullptr);
        retVal |= clFinish(opencl.commandQueue);
        retVal |= clReleaseMemObject(buffer);
        ASSERT_EQ(CL_SUCCESS, retVal);

        // Benchmark
        for (int i = 0; i < arguments.iterations; i++) {
            cl_mem buffer = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE, sizeInBytes, nullptr, &retVal);
            retVal |= clSetKernelArg(kernel, 0, sizeof(buffer), &buffer);
            ASSERT_EQ(CL_SUCCESS, retVal);

            timer.measureStart();
            retVal |= clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, nullptr, 0, nullptr, nullptr);
            retVal |= clFlush(opencl.commandQueue);
            timer.messureEnd();
            ASSERT_EQ(CL_SUCCESS, retVal);

            retVal |= clFinish(opencl.commandQueue);
            retVal |= clReleaseMemObject(buffer);
            ASSERT_EQ(CL_SUCCESS, retVal);

            statistics.pushValue(timer.Get());
        }

        // Cleanup
        ASSERT_EQ(CL_SUCCESS, clReleaseKernel(kernel));
        ASSERT_EQ(CL_SUCCESS, clReleaseProgram(program));
    }
};

} // namespace UllsTest
