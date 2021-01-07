#include "framework/intel_product/ocl/get_intel_product_ocl.h"
#include "framework/ocl/compression_helper.h"
#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/load_binary_file.h"
#include "framework/utility/memory_constants.h"
#include "framework/utility/timer.h"
#include "memory_benchmark/definitions/read_device_mem_buffer.h"

#include <gtest/gtest.h>

static TestResult run(const ReadDeviceMemBufferArguments &arguments, Statistics &statistics) {
    if (arguments.compressed && arguments.noIntelExtensions) {
        return TestResult::DeviceNotCapable;
    }

    // Setup
    cl_int retVal;

    QueueProperties queueProperties = QueueProperties::create().setProfiling(true);
    Opencl opencl(queueProperties);

    const size_t singleSendSizeInBytes = 128U;
    const size_t numOfSends = 16U; // per 128Byte in send, 2k-4k tiles in one loop iteration
    const size_t numOfLoops = 500U;
    const auto threadTileSizeInSubgroup = singleSendSizeInBytes * numOfSends;
    const size_t subgroupSize = 8;
    size_t euNum = 0;

    ASSERT_CL_SUCCESS(clGetDeviceInfo(opencl.device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(euNum), &euNum, nullptr));

    IntelProduct intelProduct = getIntelProduct(opencl);
    IntelGen gpuGen = getIntelGen(intelProduct);

    const bool useLargeGRF = gpuGen == IntelGen::Gen12hp ? true : false;
    const std::string largeGrfOpt = useLargeGRF ? " -cl-intel-256-GRF-per-thread " : " ";
    const std::string buildOptions = std::string("-cl-std=CL2.0 -cl-mad-enable -cl-fast-relaxed-math ") +
                                     " -D NUM_SENDS=" + std::to_string(numOfSends) +
                                     " -D KERNEL_LOOP_ITERATIONS=" + std::to_string(numOfLoops) +
                                     " -D THREAD_TILE_SIZE=" + std::to_string(threadTileSizeInSubgroup) +
                                     " -D SUBGROUP_SIZE=" + std::to_string(subgroupSize) +
                                     largeGrfOpt +
                                     std::string(" ");

    // Create buffer
    const cl_mem_flags compressionHint = CompressionHelper::getCompressionFlags(arguments.compressed, arguments.noIntelExtensions);
    const cl_mem_flags memFlags = CL_MEM_READ_WRITE | compressionHint;

    auto srcCpuBuffer = std::make_unique<float[]>(arguments.size / sizeof(float));
    float *pBuff = srcCpuBuffer.get();
    float floatVal1 = 0.0f;
    for (size_t i = 0; i < arguments.size / sizeof(float);) {
        for (size_t j = 0; j < 8; j++) {
            *(pBuff++) = floatVal1 + (float)j;
        }
        floatVal1 += 10.0f;
        i += 8;
    }
    const cl_mem source = clCreateBuffer(opencl.context, memFlags | CL_MEM_COPY_HOST_PTR, arguments.size, srcCpuBuffer.get(), &retVal);

    const cl_mem destination = clCreateBuffer(opencl.context, memFlags, arguments.size, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);

    const uint32_t clearGpuBuffSize = 16 * MemoryConstants::megaByte;
    const cl_mem clearGpuBuff = clCreateBuffer(opencl.context, memFlags, clearGpuBuffSize, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Check buffers compression
    auto compressionStatus = CompressionHelper::verifyCompression(source, arguments.compressed, arguments.noIntelExtensions);
    if (compressionStatus != TestResult::Success) {
        ASSERT_CL_SUCCESS(clReleaseMemObject(source));
        return compressionStatus;
    }
    compressionStatus = CompressionHelper::verifyCompression(destination, arguments.compressed, arguments.noIntelExtensions);
    if (compressionStatus != TestResult::Success) {
        ASSERT_CL_SUCCESS(clReleaseMemObject(source));
        ASSERT_CL_SUCCESS(clReleaseMemObject(destination));
        return compressionStatus;
    }

    const char *programSrc =
        "__kernel void ClearCaches(__global unsigned int *pBuffer, unsigned int buffSize) {"
        "   unsigned int i = 0;"
        "   while (i++ < buffSize) {"
        "   *(pBuffer++) = 0xDEADBEAF;"
        "   }"
        "}"
        "\n#define VECTOR_SIZE 4"
        "\n#define SEND_SIZE (sizeof(float) * VECTOR_SIZE)"
        "\n__attribute__((intel_reqd_sub_group_size(SUBGROUP_SIZE)))"
        "\n__kernel void ReadOnly(__global float4 *const pSrcBuffer, __global float4 *pDstBuffer,"
        "        unsigned int sliceMask, unsigned int sliceSize, unsigned int slotMask) {"
        "   const uint gid = get_global_id(0);"
        "   const uint sid = ((get_group_id(0) * get_num_sub_groups()) + get_sub_group_id()) % slotMask;"
        "   uint startOffset = 0;"
        "   float4 _out_data = {0.0f, 0.0f, 0.0f, 0.0f};"
        "   float4 _input_data; uint j = 0, i = 0;"
        "   for (j = 0; j < KERNEL_LOOP_ITERATIONS; j++) {"
        "       i = 0; uint slotOffset = sliceSize * (j & sliceMask);"
        "       startOffset = (((THREAD_TILE_SIZE * sid) + slotOffset) / SEND_SIZE);"
        "       __attribute__((opencl_unroll_hint(NUM_SENDS)))"
        "       do {"
        "           _input_data = as_float4(intel_sub_group_block_read4((__global const uint *)((__global uint *const)(&pSrcBuffer[startOffset]))));"
        "           _out_data += _input_data; startOffset += SUBGROUP_SIZE;"
        "       } while (++i < NUM_SENDS);"
        "   }"
        "   if (_out_data.x < 0.0f) {"
        "       pDstBuffer[gid] = _out_data;"
        "   }"
        "}";

    // Create kernel
    const auto programSrcLen = strlen(programSrc);
    cl_program program = clCreateProgramWithSource(opencl.context, 1, &programSrc, &programSrcLen, &retVal);
    retVal |= clBuildProgram(program, 1, &opencl.device, buildOptions.c_str(), nullptr, nullptr);
#if 0
    if (retVal) {
        size_t numBytes = 0;
        retVal |= clGetProgramBuildInfo(program, opencl.device, CL_PROGRAM_BUILD_LOG, 0, NULL, &numBytes);
        auto buffer = std::make_unique<char[]>(numBytes);
        retVal |= clGetProgramBuildInfo(program, opencl.device, CL_PROGRAM_BUILD_LOG, numBytes, buffer.get(), &numBytes  );
        std::cout << buffer.get() << std::endl;
    }
#endif
    ASSERT_CL_SUCCESS(retVal);

    cl_kernel kernel = clCreateKernel(program, "ReadOnly", &retVal);
    ASSERT_CL_SUCCESS(retVal);
    cl_kernel clearCacheKernel = clCreateKernel(program, "ClearCaches", &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Clear L3$ Cache kernel
    const size_t clearGws = 1;
    const size_t buffSizeInInts = clearGpuBuffSize / sizeof(cl_uint);
    retVal |= clSetKernelArg(clearCacheKernel, 0, sizeof(clearGpuBuff), &clearGpuBuff);
    retVal |= clSetKernelArg(clearCacheKernel, 1, sizeof(buffSizeInInts), &buffSizeInInts);
    retVal |= clEnqueueNDRangeKernel(opencl.commandQueue, clearCacheKernel, 1, nullptr, &clearGws, NULL, 0, nullptr, nullptr);
    retVal |= clFinish(opencl.commandQueue);
    ASSERT_CL_SUCCESS(retVal);

    cl_uint sliceSize = 0, sliceMask = 1, slotMask = 1;
    const cl_uint numThreadsPerEu = (gpuGen == IntelGen::Gen12hp) ? 8 : 7;
    const cl_uint numHwThreads = (useLargeGRF ? numThreadsPerEu / 2 : 7) * static_cast<cl_uint>(euNum);

    const size_t lws = 8;
    size_t gws = numHwThreads * subgroupSize;

    if (intelProduct != IntelProduct::Unknown) {
        const cl_uint atsThreasLargeGRFMode = 4;
        // check if surface can be covered with more than one slice
        if ((2 * numHwThreads * threadTileSizeInSubgroup) < static_cast<cl_uint>(arguments.size)) {
            slotMask = numHwThreads;
            sliceSize = numHwThreads * threadTileSizeInSubgroup;
            const auto numMaxSlices = static_cast<cl_uint>(arguments.size) / sliceSize;
            // can cover with more than one slice for all threads
            sliceMask = 1;
            do {
                sliceMask *= 2;
            } while (sliceMask <= numMaxSlices);

            sliceMask /= 2;
            sliceMask -= 1;
        } else {
            slotMask = static_cast<cl_uint>(arguments.size) / threadTileSizeInSubgroup;
            slotMask -= 1;
        }
    } else {
        //tbd for comp
        ASSERT_CL_SUCCESS(-1);
    }

    retVal |= clSetKernelArg(kernel, 0, sizeof(source), &source);
    retVal |= clSetKernelArg(kernel, 1, sizeof(destination), &destination);
    retVal |= clSetKernelArg(kernel, 2, sizeof(sliceMask), &sliceMask);
    retVal |= clSetKernelArg(kernel, 3, sizeof(sliceSize), &sliceSize);
    retVal |= clSetKernelArg(kernel, 4, sizeof(slotMask), &slotMask);
    ASSERT_CL_SUCCESS(retVal);

    //warmup
    retVal |= clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, &lws, 0, nullptr, nullptr);
    retVal |= clFinish(opencl.commandQueue);
    ASSERT_CL_SUCCESS(retVal);

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        cl_event evt;
        cl_ulong enqstart = 0;
        cl_ulong enqend = 0;

        retVal |= clEnqueueNDRangeKernel(opencl.commandQueue, clearCacheKernel, 1, nullptr, &clearGws, NULL, 0, nullptr, nullptr);
        retVal |= clFinish(opencl.commandQueue);
        ASSERT_CL_SUCCESS(retVal);

        retVal |= clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, &lws, 0, nullptr, &evt);
        retVal |= clWaitForEvents(1, &evt);
        ASSERT_CL_SUCCESS(retVal);

        retVal |= clGetEventProfilingInfo(evt, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &enqstart, NULL);
        retVal |= clGetEventProfilingInfo(evt, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &enqend, NULL);
        ASSERT_CL_SUCCESS(retVal);

        const auto time = std::chrono::nanoseconds(enqend - enqstart);
        const size_t groupsExed = gws / subgroupSize;
        const size_t totalAccessedMemory = (groupsExed * threadTileSizeInSubgroup * numOfLoops);

        statistics.pushValue(time, arguments.size);
        ASSERT_CL_SUCCESS(clReleaseEvent(evt));
    }

    // Cleanup
    ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
    ASSERT_CL_SUCCESS(clReleaseKernel(clearCacheKernel));
    ASSERT_CL_SUCCESS(clReleaseProgram(program));
    ASSERT_CL_SUCCESS(clReleaseMemObject(destination));
    ASSERT_CL_SUCCESS(clReleaseMemObject(source));
    ASSERT_CL_SUCCESS(clReleaseMemObject(clearGpuBuff));

    return TestResult::Success;
}

static RegisterTestCaseImplementation<ReadDeviceMemBuffer> registerTestCase(run, Api::OpenCL);
