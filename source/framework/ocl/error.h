#pragma once

#include "framework/utility/error.h"

#include <gtest/gtest.h>
#include <string>

#define ASSERT_CL_SUCCESS(retVal)                    \
    {                                                \
        const auto tempVarForDefine = (retVal);      \
        if (tempVarForDefine != CL_SUCCESS) {        \
            EXPECT_EQ(CL_SUCCESS, tempVarForDefine); \
            return TestResult::Error;                \
        }                                            \
    }

#define EXPECT_CL_SUCCESS(retVal)                    \
    {                                                \
        const auto tempVarForDefine = (retVal);      \
        if (tempVarForDefine != CL_SUCCESS) {        \
            EXPECT_EQ(CL_SUCCESS, tempVarForDefine); \
        }                                            \
    }

#define CL_SUCCESS_OR_RETURN_VALUE(retVal, value) \
    if ((retVal) != CL_SUCCESS) {                 \
        return (value);                           \
    }

#define CL_SUCCESS_OR_RETURN_FALSE(retVal) \
    CL_SUCCESS_OR_RETURN_VALUE((retVal), false)

#define CL_SUCCESS_OR_RETURN(retVal)                                    \
    {                                                                   \
        const auto tempVarForDefine = (retVal);                         \
        CL_SUCCESS_OR_RETURN_VALUE(tempVarForDefine, tempVarForDefine); \
    }

#define CL_SUCCESS_OR_ERROR(retVal, message)                                                                                                \
    {                                                                                                                                       \
        const auto tempVarForDefine = (retVal);                                                                                             \
        if (tempVarForDefine != CL_SUCCESS) {                                                                                               \
            const auto _message = std::string(message) + " (Fatal OpenCL error occurred, retVal=" + std::to_string(tempVarForDefine) + ")"; \
            ERROR(_message);                                                                                                                \
        }                                                                                                                                   \
    }
