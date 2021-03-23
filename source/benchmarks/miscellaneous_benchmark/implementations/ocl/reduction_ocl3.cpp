#include "framework/ocl/opencl.h"
#include "framework/ocl/utility/profiling_helper.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"

#include "definitions/reduction3.h"

TestResult run(const ReductionArguments3 &arguments, Statistics &statistics) {
    QueueProperties queueProperties = QueueProperties::create().setProfiling(true);
    Opencl opencl(queueProperties);
    cl_int retVal;

    // Create kernel
    std::string kernelSource = R"(
       __kernel void reduction(__global uint16 *results) {
            uint16 values = results[get_global_id(0)];
            uint value = values.s0 + values.s1 + values.s2 + values.s3+ values.s4 + values.s5 + values.s6+ values.s7 + values.s8 + values.s9+ values.sa + values.sb + values.sc+ values.sd + values.se + values.sf;
            local uint localSums[512u];
            localSums[get_local_id(0)] = value;
            barrier(CLK_LOCAL_MEM_FENCE);
            if(get_local_id(0)==0){
                uint sum = 0u;
                for(int i = 0 ; i < 512u ; i++ ){
                    sum += localSums[i];
                }
                atomic_add(&(((global uint*)results)[get_global_size(0)*16]), sum);
            }
       }
    )";
    const char *pKernelSource = kernelSource.c_str();
    const size_t kernelSizes = kernelSource.size();
    cl_program program = clCreateProgramWithSource(opencl.context, 1, &pKernelSource, &kernelSizes, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    retVal = clBuildProgram(program, 1, &opencl.device, nullptr, nullptr, nullptr);

    if (retVal) {
        size_t numBytes = 0;
        retVal |= clGetProgramBuildInfo(program, opencl.device, CL_PROGRAM_BUILD_LOG, 0, NULL, &numBytes);
        auto buffer = std::make_unique<char[]>(numBytes);
        retVal |= clGetProgramBuildInfo(program, opencl.device, CL_PROGRAM_BUILD_LOG, numBytes, buffer.get(), &numBytes);
        std::cout << buffer.get() << std::endl;
    }

    cl_kernel kernel = clCreateKernel(program, "reduction", &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Prepare data
    const size_t sizeInBytes = (arguments.numberOfElements + 1) * sizeof(int);
    auto lastIndexOffset = arguments.numberOfElements * sizeof(int);
    auto data = std::make_unique<int[]>(arguments.numberOfElements + 1);
    size_t expectedSum = 0u;
    size_t value = 0u;
    for (int i = 0; i < arguments.numberOfElements; i++) {
        value++;
        if (value > 4)
            value = 0;
        data[i] = static_cast<int>(value);
        expectedSum += value;
    }
    data[arguments.numberOfElements] = 0u;

    // Create buffer
    cl_mem buffer = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeInBytes, data.get(), &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Validate results
    int actualSum;
    cl_event profilingEvent{};
    cl_ulong timeNs{};

    // Warmup kernel
    const size_t gws = arguments.numberOfElements / 16;
    const size_t lws = 512u;
    ASSERT_CL_SUCCESS(clSetKernelArg(kernel, 0, sizeof(buffer), &buffer));
    ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, &lws, 0, nullptr, &profilingEvent));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    ASSERT_CL_SUCCESS(clEnqueueReadBuffer(opencl.commandQueue, buffer, true, lastIndexOffset, 4u, &actualSum, 0u, nullptr, nullptr));
    if (actualSum != expectedSum) {
        printf("\n data verification failure");
        ASSERT_CL_SUCCESS(clReleaseMemObject(buffer));
        ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
        ASSERT_CL_SUCCESS(clReleaseProgram(program));
        return TestResult::VerificationFail;
    }

    ASSERT_CL_SUCCESS(ProfilingHelper::getEventDurationInNanoseconds(profilingEvent, timeNs));
    ASSERT_CL_SUCCESS(clReleaseEvent(profilingEvent));
    cl_ulong totalTime = timeNs;
    statistics.pushValue(std::chrono::nanoseconds{timeNs});

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        int zero = 0u;
        ASSERT_CL_SUCCESS(clEnqueueWriteBuffer(opencl.commandQueue, buffer, true, lastIndexOffset, 4u, &zero, 0u, nullptr, nullptr));
        ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, &lws, 0, nullptr, &profilingEvent));
        ASSERT_CL_SUCCESS(clWaitForEvents(1, &profilingEvent));
        ASSERT_CL_SUCCESS(ProfilingHelper::getEventDurationInNanoseconds(profilingEvent, timeNs));
        totalTime += timeNs;
        ASSERT_CL_SUCCESS(clReleaseEvent(profilingEvent));
        if (i + 1 < arguments.iterations)
            statistics.pushValue(std::chrono::nanoseconds{timeNs});
    }

    uint32_t iterationCount = arguments.iterations + 1;
    double timePerIteration = (double)totalTime / (double)iterationCount;

    printf("\n Variant 3 samples gathered %d , element count %lu , timePerIteration (us) %f Bandwidth (GB/s) %f \n", iterationCount, (size_t)arguments.numberOfElements, timePerIteration / 1000.0, sizeInBytes / timePerIteration);

    ASSERT_CL_SUCCESS(clReleaseMemObject(buffer));
    ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
    ASSERT_CL_SUCCESS(clReleaseProgram(program));

    return TestResult::Success;
}

static RegisterTestCaseImplementation<Reduction3> registerTestCase(run, Api::OpenCL);
