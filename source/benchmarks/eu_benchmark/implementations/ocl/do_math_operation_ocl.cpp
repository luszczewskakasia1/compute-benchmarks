#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/compiler_options_builder.h"
#include "framework/utility/file_helper.h"
#include "framework/utility/math_operation_helper.h"
#include "framework/utility/timer.h"

#include "definitions/do_math_operation.h"

#include <cstring>
#include <gtest/gtest.h>

std::string getCompilerOptions(DataType dataType, MathOperation operation) {
    CompilerOptionsBuilder options{};
    options.addDefinitionKeyValue("DATATYPE", DataTypeHelper::toOpenclC(dataType));

    switch (operation) {
    case MathOperation::Add:
        options.addMacro("MATH_OPERATION", {"a", "b"}, "a=a+b");
        break;
    case MathOperation::Sub:
        options.addMacro("MATH_OPERATION", {"a", "b"}, "a=a-b");
        break;
    case MathOperation::Div:
        options.addMacro("MATH_OPERATION", {"a", "b"}, "a=a/b");
        break;
    case MathOperation::Modulo:
        options.addMacro("MATH_OPERATION", {"a", "b"}, "a=a%b");
        break;
    case MathOperation::Inc:
        options.addMacro("MATH_OPERATION", {"a", "b"}, "a++");
        break;
    case MathOperation::Dec:
        options.addMacro("MATH_OPERATION", {"a", "b"}, "a--");
        break;
    case MathOperation::Min:
        options.addMacro("MATH_OPERATION", {"a", "b"}, "a=min(a,b)");
        break;
    case MathOperation::Max:
        options.addMacro("MATH_OPERATION", {"a", "b"}, "a=max(a,b)");
        break;
    case MathOperation::And:
        options.addMacro("MATH_OPERATION", {"a", "b"}, "a=a&b");
        break;
    case MathOperation::Or:
        options.addMacro("MATH_OPERATION", {"a", "b"}, "a=a|b");
        break;
    case MathOperation::Xor:
        options.addMacro("MATH_OPERATION", {"a", "b"}, "a=a^b");
        break;
    default:
        FATAL_ERROR("Invalid math operation");
    }

    return options.str();
}

static TestResult run(const DoMathOperationArguments &arguments, Statistics &statistics) {
    // Check support
    if (!MathOperationHelper::isSupportedAsNormal(arguments.operation, arguments.dataType)) {
        return TestResult::DeviceNotCapable;
    }

    // Setup
    Opencl opencl{};
    Timer timer{};
    cl_int retVal{};

    // Prepare data
    const size_t lws = arguments.workgroupSize;
    const size_t gws = arguments.workgroupSize * arguments.workgroupCount;
    const size_t loopIterations = 200u;                   // tweakable constant
    const size_t operationsPerLoop = 128u;                // non-tweakable constant, this is hardcoded in the kernel code
    const size_t enqueueCount = arguments.iterations + 1; // non-tweakable constant
    const auto data = MathOperationHelper::generateTestData(arguments.dataType, arguments.operation, loopIterations, operationsPerLoop, enqueueCount);

    // Create and initialize the buffer with test data
    cl_mem buffer = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE, gws * data.sizeOfDataType, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    ASSERT_CL_SUCCESS(clEnqueueWriteBuffer(opencl.commandQueue, buffer, CL_FALSE, 0, data.sizeOfDataType, data.initialValue, 0, nullptr, nullptr));

    // Create kernel
    const std::vector<uint8_t> kernelSource = FileHelper::loadTextFile("eu_benchmark_perform_math_operation.cl");
    if (kernelSource.size() == 0) {
        return TestResult::KernelNotFound;
    }
    const char *source = reinterpret_cast<const char *>(kernelSource.data());
    const size_t sourceLength = kernelSource.size();
    cl_program program = clCreateProgramWithSource(opencl.context, 1, &source, &sourceLength, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    const std::string compilerOptions = getCompilerOptions(arguments.dataType, arguments.operation);
#if 0
    auto ret = clBuildProgram(program, 1, &opencl.device, compilerOptions.c_str(), nullptr, nullptr);
    size_t size{};
    ASSERT_CL_SUCCESS(clGetProgramBuildInfo(program, opencl.device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &size));
    auto b = std::make_unique<char[]>(size);
    ASSERT_CL_SUCCESS(clGetProgramBuildInfo(program, opencl.device, CL_PROGRAM_BUILD_LOG, size, b.get(), nullptr));
    std::cout << b << '\n';
    FATAL_ERROR("DEBUG");
#endif
    ASSERT_CL_SUCCESS(clBuildProgram(program, 1, &opencl.device, compilerOptions.c_str(), nullptr, nullptr));
    cl_kernel kernel = clCreateKernel(program, "do_math_operation", &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Warmup
    ASSERT_CL_SUCCESS(clSetKernelArg(kernel, 0, sizeof(buffer), &buffer));
    ASSERT_CL_SUCCESS(clSetKernelArg(kernel, 1, data.sizeOfDataType, data.otherArgument));
    ASSERT_CL_SUCCESS(clSetKernelArg(kernel, 2, sizeof(data.loopIterations), &data.loopIterations));
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
    std::byte result[8] = {};
    ASSERT_CL_SUCCESS(clEnqueueReadBuffer(opencl.commandQueue, buffer, CL_BLOCKING, 0, data.sizeOfDataType, result, 0, nullptr, nullptr));
    if (std::memcmp(result, data.expectedValue, data.sizeOfDataType) != 0) {
        return TestResult::VerificationFail;
    }

    // Cleanup
    ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
    ASSERT_CL_SUCCESS(clReleaseProgram(program));
    ASSERT_CL_SUCCESS(clReleaseMemObject(buffer));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<DoMathOperation> registerTestCase(run, Api::OpenCL);
