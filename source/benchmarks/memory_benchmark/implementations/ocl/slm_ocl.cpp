/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021 Intel Corporation
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit this
 * software or the related documents without Intel's prior written permission.
 *
 * This software and the related documents are provided as is, with no express or
 * implied warranties, other than those that are expressly stated in the License.
 */

#include "framework/ocl/intel_product/get_intel_product_ocl.h"
#include "framework/ocl/opencl.h"
#include "framework/ocl/utility/compression_helper.h"
#include "framework/ocl/utility/profiling_helper.h"
#include "framework/ocl/utility/program_helper_ocl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/compiler_options_builder.h"
#include "framework/utility/file_helper.h"
#include "framework/utility/memory_constants.h"
#include "framework/utility/timer.h"

#include "definitions/slm.h"

#include <gtest/gtest.h>
#include <numeric>

static TestResult run(const SlmTrafficArguments &arguments, Statistics &statistics) {
    if (arguments.writeDirection == true) {
        return TestResult::DeviceNotCapable;
    }

    // Setup
    cl_int retVal;
    QueueProperties queueProperties = QueueProperties::create().setProfiling(true);
    Opencl opencl(queueProperties);

    // Check platform we're on
    IntelProduct intelProduct = getIntelProduct(opencl);
    IntelGen gpuGen = getIntelGen(intelProduct);
    if (gpuGen == IntelGen::Unknown) {
        return TestResult::DeviceNotCapable; //tbd for comp
    }
    const cl_uint numThreadsPerEu = (gpuGen >= IntelGen::XeHpCore) ? 8 : 7;

    const size_t numOfSends = 10U;
    const size_t numOfLoops = 128U;
    const size_t lws = 256;
    const size_t vectorSize = 4;
    const bool printPerEnqueueStats = false;
    const bool buildFromFile = false;
    size_t euNum = 0;
    size_t gpuFreq = 0;

    ASSERT_CL_SUCCESS(clGetDeviceInfo(opencl.device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(euNum), &euNum, nullptr));
    ASSERT_CL_SUCCESS(clGetDeviceInfo(opencl.device, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(gpuFreq), &gpuFreq, nullptr));

    const size_t workSize = euNum * numThreadsPerEu * 16;
    const size_t inOutBuffSize = workSize * vectorSize * sizeof(uint32_t);

    CompilerOptionsBuilder buildOptions{};
    buildOptions.addOption("-cl-no-signed-zeros ");
    buildOptions.addOption("-cl-std=CL2.0 ");
    buildOptions.addOption("-cl-mad-enable ");
    buildOptions.addOption("-cl-fast-relaxed-math ");
    buildOptions.addDefinitionKeyValue("KERNEL_LOOP_ITERATIONS", numOfLoops);

    // Create buffer
    const cl_mem_flags memFlagsIn = CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR;
    const cl_mem_flags memFlagsOut = CL_MEM_READ_WRITE;

    auto hostInBuffer = std::make_unique<int[]>(inOutBuffSize / sizeof(int));
    int *pBuff = hostInBuffer.get();
    for (size_t i = 0; i < inOutBuffSize / sizeof(int); i++) {
        *(pBuff++) = (int)i;
    }
    const cl_mem source = clCreateBuffer(opencl.context, memFlagsIn, inOutBuffSize, hostInBuffer.get(), &retVal);
    ASSERT_CL_SUCCESS(retVal);

    const cl_mem destination = clCreateBuffer(opencl.context, memFlagsOut, inOutBuffSize, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);

    const size_t cryptKeySize = (size_t)(60 * 4);
    auto srcHostBuffKey = std::make_unique<char[]>(cryptKeySize);
    char *pBuffC = srcHostBuffKey.get();
    for (size_t i = 0; i < cryptKeySize; i++) {
        *(pBuffC++) = (char)i;
    }
    const cl_mem keyBuff = clCreateBuffer(opencl.context, memFlagsIn, cryptKeySize, srcHostBuffKey.get(), &retVal);
    ASSERT_CL_SUCCESS(retVal);

    const size_t cryptTableSize = (size_t)(5140);
    auto hostBuffTable = std::make_unique<int[]>(cryptTableSize);
    int *pBuffI = hostBuffTable.get();
    for (size_t i = 0; i < cryptKeySize; i++) {
        *(pBuffI++) = (int)i;
    }

    const cl_mem tableBuff = clCreateBuffer(opencl.context, memFlagsIn, cryptTableSize, hostBuffTable.get(), &retVal);
    ASSERT_CL_SUCCESS(retVal);

    const char *programSlmRead = NULL;
    size_t programSize = 0;
    if (!buildFromFile) {
        programSlmRead =
            "\n#define AES_BLOCK_SIZE 16  "
            "\n#define AES_TABLE_SIZE 256 "
            "\n#define AES_TABLE_MAX 5    "
            "\n#define AES_CONST_SIZE (AES_TABLE_SIZE * AES_TABLE_MAX)    "
            "\n#ifndef INTEL_INTERNAL_DEBUG_H_INCLUDED    "
            "\n#define INTEL_INTERNAL_DEBUG_H_INCLUDED    "
            "\n            ulong __attribute__((overloadable)) intel_get_cycle_counter(void);    "
            "\n#endif    "

            "\ninline uint4 Load4(    "
            "    __local uint * pData,    "
            "    const uint4 uX, const uint iY) {    "
            "    const uint uExtent = iY * AES_TABLE_SIZE;    "
            "    const uint4 uNdx = uX + uExtent;    "

            "    return (uint4)(pData[uNdx.x], pData[uNdx.y], pData[uNdx.z], pData[uNdx.w]);    "
            "}    "

            "\n__attribute__((intel_reqd_sub_group_size(16)))    "
            "\n__kernel void    "
            "slmRead_CryptBased(__global const uint4 *restrict pInput, __global uint4 *restrict pOutput,    "
            "                   __global const uint *restrict pTables, __constant uint4 *pKey, const uint iRounds) {    "
            "    const size_t iNdx = get_global_id(0);    "
            "\n    #define Load4T(x, y) Load4(ulTables, x, y)    "
            "\n    __local uint ulTables[AES_CONST_SIZE];    "
            "\n    uint4 perfData = (uint4)(0, (uint)-1, 0, 0); //avg, min, max, isOverflow    "

            "\n    const uint iLdx = get_local_id(0);    "
            "\n    if (iLdx < AES_TABLE_SIZE) {    "
            "\n        const uint WORK_GROUP_SIZE = get_local_size(0);    "
            "\n        const uint COPY_TABLE_SIZE = min(WORK_GROUP_SIZE, (uint)AES_TABLE_SIZE);    "
            "\n        const uint COPY_TABLE_COUNT = AES_CONST_SIZE / COPY_TABLE_SIZE;    "
            "\n        for (uint i = 0; i < COPY_TABLE_COUNT; i++) {    "
            "\n           const uint uTbNdx = iLdx + i * COPY_TABLE_SIZE;    "
            "\n            ulTables[uTbNdx] = pTables[uTbNdx];    "
            "\n        }    "
            "\n    }    "
            "\n    barrier(CLK_LOCAL_MEM_FENCE);    "
            "\n    uint4 state = (uint4)(0, 0, 0, 0);    "
            "\n    if (iRounds > 0xFF) {    "
            "\n        state = pInput[iNdx] ^ pKey[0];    "
            "\n    }    "
            "\n    uint4 istate, tstate;    "
            "\n    uint nLoop = 0;    "
            "\n    if (iRounds < 0xFF) {    "
            "\n        nLoop = KERNEL_LOOP_ITERATIONS;    "
            "\n    }    "
            "\n    ulong timerON = 0;    "
            "\n    ulong timerOFF = 0, time = 0;    "
            "\n    for (uint j = 0; j < nLoop; j++) {    "
            "\n        timerON = intel_get_cycle_counter();    "
            "\n        for (uint i = 1; i < iRounds; i++) {    "
            "\n            istate = state & 0xFF;    "
            "\n            tstate = Load4T(istate.xyzw, 0);    "
            "\n            istate = (state >> 8) & 0xFF;    "
            "\n            tstate ^= Load4T(istate.yzwx, 1);    "
            "\n            istate = (state >> 16) & 0xFF;    "
            "\n            tstate ^= Load4T(istate.zwxy, 2);    "
            "\n            istate = state >> 24;    "
            "\n            tstate ^= Load4T(istate.wxyz, 3);    "
            "\n            state = tstate ^ pKey[i];    "
            "\n        }    "
            "\n        timerOFF = intel_get_cycle_counter();    "
            "\n        time = ((timerOFF - timerON)) / (iRounds * 4);    "
            "\n        if (((time >> 32) & 0xFFFFFFFF) > 0) {    "
            "\n            time = 0xFFFFFFFF;    "
            "\n            perfData[3] = 1U; //overflow    "
            "\n        }    "
            "\n       perfData[0] = ((perfData[0] * j) + (uint)time) / (j + 1); //cumulative avgerage    "
            "\n        perfData[1] = min(perfData[1], (uint)time);               //min    "
            "\n        perfData[2] = max(perfData[2], (uint)time);               //max    "
            "\n    }    "
            "\n    istate = state & 0xFF;    "
            "\n    tstate = Load4T(istate.xyzw, 4);    "
            "\n    istate = (state >> 8) & 0xFF;    "
            "\n    tstate |= Load4T(istate.yzwx, 4) << 8;    "
            "\n    istate = (state >> 16) & 0xFF;    "
            "\n    tstate |= Load4T(istate.zwxy, 4) << 16;    "
            "\n    istate = state >> 24;    "
            "\n    tstate |= Load4T(istate.wxyz, 4) << 24;    "
            "\n    if (iRounds > 0xFF)    "
            "\n        pOutput[iNdx] = tstate ^ pKey[iRounds];    "
            "\n    else {    "
            "\n        pOutput[iNdx] = perfData;    "
            "\n    }    "
            "\n}    ";
        programSize = strlen(programSlmRead);
    } else {
        const auto programSrc = FileHelper::loadBinaryFile("slmRead_CryptKernelBased.cl");
        if (programSrc.size() == 0) {
            return TestResult::KernelNotFound;
        }

        programSlmRead = reinterpret_cast<const char *>(programSrc.data());
        programSize = programSrc.size();
    }
    cl_program program{};
    if (TestResult result = ProgramHelperOcl::buildProgramFromSource(opencl.context, opencl.device, programSlmRead, programSize, buildOptions.str().c_str(), program); result != TestResult::Success) {
        if (result != TestResult::Success) {
            size_t numBytes = 0;
            retVal |= clGetProgramBuildInfo(program, opencl.device, CL_PROGRAM_BUILD_LOG, 0, NULL, &numBytes);
            auto buffer = std::make_unique<char[]>(numBytes);
            retVal |= clGetProgramBuildInfo(program, opencl.device, CL_PROGRAM_BUILD_LOG, numBytes, buffer.get(), &numBytes);
            std::cout << buffer.get() << std::endl;
        }

        return TestResult::KernelBuildError;
    }
    cl_kernel kernel = clCreateKernel(program, "slmRead_CryptBased", &retVal);
    ASSERT_CL_SUCCESS(retVal);

    cl_uint nRounds = numOfSends;
    size_t gws = workSize / arguments.occupancyDivider;

    //std::cout << "SLM Read latency test (SLM size " << arguments.size/1024 << "kB)" << std::endl;
    {

        //std::cout << "\nGWS " << gws << " LWS "<< lws <<" H/W utilization: totalEUs/" << arguments.occupancyDivider << std::endl;

        ASSERT_CL_SUCCESS(clSetKernelArg(kernel, 0, sizeof(source), &source));
        ASSERT_CL_SUCCESS(clSetKernelArg(kernel, 1, sizeof(destination), &destination));
        ASSERT_CL_SUCCESS(clSetKernelArg(kernel, 2, sizeof(tableBuff), &tableBuff));
        ASSERT_CL_SUCCESS(clSetKernelArg(kernel, 3, sizeof(keyBuff), &keyBuff));
        ASSERT_CL_SUCCESS(clSetKernelArg(kernel, 4, sizeof(nRounds), &nRounds));
        ASSERT_CL_SUCCESS(retVal);

        // Benchmark
        for (auto i = 0u; i < arguments.iterations; i++) {
            cl_event evt;

            ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, &lws, 0, nullptr, &evt));
            ASSERT_CL_SUCCESS(clWaitForEvents(1, &evt));

            const size_t dataReadSize = gws * vectorSize * sizeof(uint32_t);
            auto slmClocksBuffer = std::make_unique<uint32_t[]>(dataReadSize);

            retVal |= clEnqueueReadBuffer(opencl.commandQueue, destination, true, 0, dataReadSize, slmClocksBuffer.get(), 0, nullptr, nullptr);
            retVal |= clFinish(opencl.commandQueue);
            ASSERT_CL_SUCCESS(retVal);

            std::vector<size_t> vecPerThreadAverages;
            std::vector<size_t> vecPerThreadMins;
            std::vector<size_t> vecPerThreadMax;
            bool printToConsole = false;
            for (size_t item = 0; item < gws; item++) {
                size_t avgInThread = static_cast<size_t>(slmClocksBuffer.get()[(item * vectorSize)]);
                size_t minInThread = static_cast<size_t>(slmClocksBuffer.get()[(item * vectorSize) + 1]);
                size_t maxInThread = static_cast<size_t>(slmClocksBuffer.get()[(item * vectorSize) + 2]);
                bool isOverflow = (bool)slmClocksBuffer.get()[(item * vectorSize) + 3];
                if (isOverflow) {
                    if (printToConsole == false) {
                        std::cout << "BW overflow happens in gws " << gws << std::endl;
                        printToConsole = true;
                    }
                    continue;
                }
                vecPerThreadAverages.push_back(avgInThread);
                vecPerThreadMins.push_back(minInThread);
                vecPerThreadMax.push_back(maxInThread);
            }

            std::chrono::steady_clock::duration averageLatency = static_cast<std::chrono::steady_clock::duration>(std::accumulate(vecPerThreadAverages.begin(), vecPerThreadAverages.end(), (size_t)0) / vecPerThreadAverages.size());

            if (printPerEnqueueStats) {
                size_t minInAllThreads = *std::min_element(vecPerThreadMins.begin(), vecPerThreadMins.end());
                size_t maxInAllThreads = *std::max_element(vecPerThreadMax.begin(), vecPerThreadMax.end());
                std::cout << "[" << i << "] Average Latency " << static_cast<size_t>(std::accumulate(vecPerThreadAverages.begin(), vecPerThreadAverages.end(), (size_t)0) / vecPerThreadAverages.size()) << " [clk]  min: " << minInAllThreads << " [clk]  max: " << maxInAllThreads << " [clk] (SLM uint4 load.slm.d32)" << std::endl;
            }
            statistics.pushValue(averageLatency, MeasurementUnit::Latency, MeasurementType::Gpu);

            cl_ulong timeNs{};
            ASSERT_CL_SUCCESS(ProfilingHelper::getEventDurationInNanoseconds(evt, timeNs));
            ASSERT_CL_SUCCESS(clReleaseEvent(evt));
        }
    }
    // Cleanup
    ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
    ASSERT_CL_SUCCESS(clReleaseProgram(program));
    ASSERT_CL_SUCCESS(clReleaseMemObject(destination));
    ASSERT_CL_SUCCESS(clReleaseMemObject(source));
    ASSERT_CL_SUCCESS(clReleaseMemObject(tableBuff));
    ASSERT_CL_SUCCESS(clReleaseMemObject(keyBuff));

    return TestResult::Success;
}

static RegisterTestCaseImplementation<SlmTraffic> registerTestCase(run, Api::OpenCL);
