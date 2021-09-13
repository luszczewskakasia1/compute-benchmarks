#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/file_helper.h"
#include "framework/utility/math_operation_helper.h"
#include "framework/utility/timer.h"

#include "definitions/separate_atomic.h"
#include "kernel_helper.h"

#include <cstring>
#include <gtest/gtest.h>

static TestResult run(const SeparateAtomicsArguments &arguments, Statistics &statistics) {
    // Setup
    Opencl opencl{};
    Timer timer{};
    cl_int retVal{};

    // Check support
    if (!MathOperationHelper::isSupportedAsAtomic(arguments.atomicOperation, arguments.dataType, opencl.getExtensions().isGlobalFloatAtomicsSupported())) {
        return TestResult::DeviceNotCapable;
    }

    // Prepare data
    const size_t lws = arguments.workgroupSize;
    const size_t gws = arguments.workgroupSize * arguments.workgroupCount;
    const size_t threadsPerAtomicCount = arguments.iterations + 1;
    const auto data = KernelHelper::getDataForKernel(arguments.dataType, arguments.atomicOperation, threadsPerAtomicCount);

    // Create and initialize the buffer with atomics
    const size_t cachelinesCount = gws / arguments.atomicsPerCacheline;
    const size_t totalBufferSize = cachelinesCount * MemoryConstants::cachelineSize;
    cl_mem buffer = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE, totalBufferSize, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    ASSERT_CL_SUCCESS(clEnqueueFillBuffer(opencl.commandQueue, buffer, data.initialValue, data.sizeOfDataType, 0, totalBufferSize, 0, nullptr, nullptr));

    // Create and initialize the buffer with other argument
    const size_t otherArgumentsBufferSize = 32u;
    cl_mem otherArgumentsBuffer = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE, data.sizeOfDataType * otherArgumentsBufferSize, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    ASSERT_CL_SUCCESS(clEnqueueFillBuffer(opencl.commandQueue, otherArgumentsBuffer, data.otherArgument, data.sizeOfDataType, 0, data.sizeOfDataType * otherArgumentsBufferSize, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Create kernel
    const std::vector<uint8_t> kernelSource = FileHelper::loadTextFile("atomic_benchmark_kernel.cl");
    if (kernelSource.size() == 0) {
        return TestResult::KernelNotFound;
    }
    const char *source = reinterpret_cast<const char *>(kernelSource.data());
    const size_t sourceLength = kernelSource.size();
    cl_program program = clCreateProgramWithSource(opencl.context, 1, &source, &sourceLength, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    const std::string compilerOptions = KernelHelper::getCompilerOptions(arguments.dataType, arguments.atomicOperation, otherArgumentsBufferSize);
    ASSERT_CL_COMPILATION_SUCCESS(clBuildProgram(program, 1, &opencl.device, compilerOptions.c_str(), nullptr, nullptr));

    cl_kernel kernel = clCreateKernel(program, "separate_atomics", &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Warmup
    ASSERT_CL_SUCCESS(clSetKernelArg(kernel, 0, sizeof(buffer), &buffer));
    ASSERT_CL_SUCCESS(clSetKernelArg(kernel, 1, sizeof(otherArgumentsBuffer), &otherArgumentsBuffer));
    ASSERT_CL_SUCCESS(clSetKernelArg(kernel, 2, sizeof(data.loopIterations), &data.loopIterations));
    ASSERT_CL_SUCCESS(clSetKernelArg(kernel, 3, sizeof(arguments.atomicsPerCacheline.getSizeOf()), arguments.atomicsPerCacheline.getAddressOf()));
    ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, &lws, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        timer.measureStart();
        ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, &lws, 0, nullptr, nullptr));
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));
        timer.measureEnd();
        statistics.pushValue(timer.get(), MeasurementUnit::Microseconds, MeasurementType::Cpu);
    }

    // Verify
    auto result = std::make_unique<std::byte[]>(totalBufferSize);
    ASSERT_CL_SUCCESS(clEnqueueReadBuffer(opencl.commandQueue, buffer, CL_BLOCKING, 0, totalBufferSize, result.get(), 0, nullptr, nullptr));
    for (auto cachelineIndex = 0u; cachelineIndex < cachelinesCount; cachelineIndex++) {
        for (auto atomicIndex = 0u; atomicIndex < MemoryConstants::cachelineSize / data.sizeOfDataType; atomicIndex++) {
            const bool wasTouched = atomicIndex < arguments.atomicsPerCacheline;
            const std::byte *expectedValue = wasTouched ? data.expectedValue : data.initialValue;
            const std::byte *actualValue = result.get() + cachelineIndex * MemoryConstants::cachelineSize + atomicIndex * data.sizeOfDataType;
            if (std::memcmp(actualValue, expectedValue, data.sizeOfDataType) != 0) {
                return TestResult::VerificationFail;
            }
        }
    }

    // Cleanup
    ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
    ASSERT_CL_SUCCESS(clReleaseProgram(program));
    ASSERT_CL_SUCCESS(clReleaseMemObject(buffer));
    ASSERT_CL_SUCCESS(clReleaseMemObject(otherArgumentsBuffer));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<SeparateAtomics> registerTestCase(run, Api::OpenCL);
