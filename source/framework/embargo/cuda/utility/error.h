/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2025-2026 Intel Corporation
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

#pragma once

#include "framework/embargo/cuda/utility/error_codes.h"
#include "framework/test_case/test_result.h"
#include "framework/utility/error.h"

#include <string>

#define ASSERT_CUDA_DRIVER_RESULT_SUCCESS(retVal)                                                                                                         \
    {                                                                                                                                                     \
        const CUresult tempVarForDefine = (retVal);                                                                                                       \
        if (tempVarForDefine != CUDA_SUCCESS) {                                                                                                           \
            NON_FATAL_ERROR("ASSERT_CUDA_DRIVER_RESULT_SUCCESS", #retVal, std::to_string(tempVarForDefine).c_str(), cudaErrorToString(tempVarForDefine)); \
            return TestResult::Error;                                                                                                                     \
        }                                                                                                                                                 \
    }

#define EXPECT_CUDA_DRIVER_RESULT_SUCCESS(retVal)                                                                                                         \
    {                                                                                                                                                     \
        const CUresult tempVarForDefine = (retVal);                                                                                                       \
        if (tempVarForDefine != CUDA_SUCCESS) {                                                                                                           \
            NON_FATAL_ERROR("EXPECT_CUDA_DRIVER_RESULT_SUCCESS", #retVal, std::to_string(tempVarForDefine).c_str(), cudaErrorToString(tempVarForDefine)); \
        }                                                                                                                                                 \
    }

#define CUDA_DRIVER_RESULT_SUCCESS_OR_RETURN_VALUE(retVal, value) \
    if ((retVal) != CUDA_SUCCESS) {                               \
        return (value);                                           \
    }

#define CUDA_DRIVER_RESULT_SUCCESS_OR_RETURN_FALSE(retVal) \
    CUDA_DRIVER_RESULT_SUCCESS_OR_RETURN_VALUE((retVal), false)

#define CUDA_DRIVER_RESULT_SUCCESS_OR_RETURN_ERROR(retVal) \
    CUDA_DRIVER_RESULT_SUCCESS_OR_RETURN_VALUE((retVal), TestResult::Error)

#define CUDA_DRIVER_RESULT_SUCCESS_OR_RETURN(retVal)                                    \
    {                                                                                   \
        const auto tempVarForDefine = (retVal);                                         \
        CUDA_DRIVER_RESULT_SUCCESS_OR_RETURN_VALUE(tempVarForDefine, tempVarForDefine); \
    }

#define CUDA_DRIVER_RESULT_SUCCESS_OR_ERROR(retVal)                                                                           \
    {                                                                                                                         \
        const auto tempVarForDefine = (retVal);                                                                               \
        if (tempVarForDefine != CUDA_SUCCESS) {                                                                               \
            const auto _message = std::string("Fatal CUDA error occurred, retVal=") + std::to_string(tempVarForDefine) + ")"; \
            FATAL_ERROR(_message);                                                                                            \
        }                                                                                                                     \
    }

#define ASSERT_CUDA_RUNTIME_RESULT_SUCCESS(retVal)                                                                                                         \
    {                                                                                                                                                      \
        const cudaError_t tempVarForDefine = (retVal);                                                                                                     \
        if (tempVarForDefine != cudaSuccess) {                                                                                                             \
            NON_FATAL_ERROR("ASSERT_CUDA_RUNTIME_RESULT_SUCCESS", #retVal, std::to_string(tempVarForDefine).c_str(), cudaErrorToString(tempVarForDefine)); \
            return TestResult::Error;                                                                                                                      \
        }                                                                                                                                                  \
    }

#define EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(retVal)                                                                                                         \
    {                                                                                                                                                      \
        const cudaError_t tempVarForDefine = (retVal);                                                                                                     \
        if (tempVarForDefine != cudaSuccess) {                                                                                                             \
            NON_FATAL_ERROR("EXPECT_CUDA_RUNTIME_RESULT_SUCCESS", #retVal, std::to_string(tempVarForDefine).c_str(), cudaErrorToString(tempVarForDefine)); \
        }                                                                                                                                                  \
    }

#define CUDA_RUNTIME_RESULT_SUCCESS_OR_RETURN_VALUE(retVal, value) \
    if ((retVal) != cudaSuccess) {                                 \
        return (value);                                            \
    }

#define CUDA_RUNTIME_RESULT_SUCCESS_OR_RETURN_FALSE(retVal) \
    CUDA_RUNTIME_RESULT_SUCCESS_OR_RETURN_VALUE((retVal), false)

#define CUDA_RUNTIME_RESULT_SUCCESS_OR_RETURN_ERROR(retVal) \
    CUDA_RUNTIME_RESULT_SUCCESS_OR_RETURN_VALUE((retVal), TestResult::Error)

#define CUDA_RUNTIME_RESULT_SUCCESS_OR_RETURN(retVal)                                    \
    {                                                                                    \
        const auto tempVarForDefine = (retVal);                                          \
        CUDA_RUNTIME_RESULT_SUCCESS_OR_RETURN_VALUE(tempVarForDefine, tempVarForDefine); \
    }

#define CUDA_RUNTIME_RESULT_SUCCESS_OR_ERROR(retVal)                                                                          \
    {                                                                                                                         \
        const auto tempVarForDefine = (retVal);                                                                               \
        if (tempVarForDefine != cudaSuccess) {                                                                                \
            const auto _message = std::string("Fatal CUDA error occurred, retVal=") + std::to_string(tempVarForDefine) + ")"; \
            FATAL_ERROR(_message);                                                                                            \
        }                                                                                                                     \
    }
