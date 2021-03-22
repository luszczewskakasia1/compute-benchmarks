#include "framework/ocl/opencl.h"
#include "framework/ocl/utility/profiling_helper.h"
#include "framework/utility/file_helper.h"
#include "framework/utility/timer.h"
#include "framework/workload/register_workload.h"

struct ReductionArguments : WorkloadArgumentContainer {
    PositiveIntegerArgument numberOfElements;

    ReductionArguments()
        : numberOfElements(*this, "numberOfElements", "Number of elements that will be reduced") {}
};

struct ReductionWorkload : Workload<ReductionArguments> {};

TestResult run(const ReductionArguments &arguments, Statistics &statistics, WorkloadSynchronization &synchronization, WorkloadIo &io) {
    QueueProperties queueProperties = QueueProperties::create().setProfiling(true);
    Opencl opencl(queueProperties);
    cl_event profilingEvent;
    Timer timer;
    cl_int retVal;

    // Create kernel
    std::string kernelSource = R"(
       
       __kernel void reduction(__global uint *results) {
            if(get_global_id(0) != 0 )
            atomic_add(&results[0], results[get_global_id(0)]);
       }
       
)";
    const char *pKernelSource = kernelSource.c_str();
    const size_t kernelSizes = kernelSource.size();
    cl_program program = clCreateProgramWithSource(opencl.context, 1, &pKernelSource, &kernelSizes, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    ASSERT_CL_SUCCESS(clBuildProgram(program, 1, &opencl.device, nullptr, nullptr, nullptr));
    cl_kernel kernel = clCreateKernel(program, "reduction", &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Prepare data
    const size_t sizeInBytes = arguments.numberOfElements * sizeof(int);
    std::vector<int> data;
    data.resize(arguments.numberOfElements);
    size_t expectedSum = 0u;
    size_t value = 0u;
    for (int i = 0; i < arguments.numberOfElements; i++) {
        value++;
        if (value > 4)
            value = 0;
        data[i] = static_cast<int>(value);
        expectedSum += value;
    }

    // Create buffer
    cl_mem buffer = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeInBytes, data.data(), &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Warmup kernel
    const size_t gws = arguments.numberOfElements;
    const size_t lws = 1;
    ASSERT_CL_SUCCESS(clSetKernelArg(kernel, 0, sizeof(buffer), &buffer));
    ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, &lws, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Validate results
    int actualSum;
    ASSERT_CL_SUCCESS(clEnqueueReadBuffer(opencl.commandQueue, buffer, true, 0u, 4u, &actualSum, 0u, nullptr, nullptr));
    if (actualSum != expectedSum) {
        ASSERT_CL_SUCCESS(clReleaseMemObject(buffer));
        ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
        ASSERT_CL_SUCCESS(clReleaseProgram(program));
        return TestResult::VerificationFail;
    }

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, nullptr, 0, nullptr, &profilingEvent));
        ASSERT_CL_SUCCESS(clWaitForEvents(1, &profilingEvent));

        cl_ulong timeNs{};
        ASSERT_CL_SUCCESS(ProfilingHelper::getEventDurationInNanoseconds(profilingEvent, timeNs));
        ASSERT_CL_SUCCESS(clReleaseEvent(profilingEvent));
        statistics.pushValue(std::chrono::nanoseconds{timeNs});
    }

    ASSERT_CL_SUCCESS(clReleaseMemObject(buffer));
    ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
    ASSERT_CL_SUCCESS(clReleaseProgram(program));

    return TestResult::Success;
}

int main(int argc, char **argv) {
    ReductionWorkload workload;
    ReductionWorkload::implementation = run;
    return workload.runFromCommandLine(argc, argv);
}
