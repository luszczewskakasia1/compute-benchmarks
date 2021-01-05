#pragma once

#include "framework/utility/error.h"

#include <gtest/gtest.h>
#include <string>

#define ASSERT_ZE_RESULT_SUCCESS(retVal)                    \
    {                                                       \
        const auto tempVarForDefine = (retVal);             \
        if (tempVarForDefine != ZE_RESULT_SUCCESS) {        \
            EXPECT_EQ(ZE_RESULT_SUCCESS, tempVarForDefine); \
            return TestResult::Error;                       \
        }                                                   \
    }

#define EXPECT_ZE_RESULT_SUCCESS(retVal)                    \
    {                                                       \
        const auto tempVarForDefine = (retVal);             \
        if (tempVarForDefine != ZE_RESULT_SUCCESS) {        \
            EXPECT_EQ(ZE_RESULT_SUCCESS, tempVarForDefine); \
        }                                                   \
    }

#define ZE_RESULT_SUCCESS_OR_RETURN_VALUE(retVal, value) \
    if ((retVal) != ZE_RESULT_SUCCESS) {                 \
        return (value);                                  \
    }

#define ZE_RESULT_SUCCESS_OR_RETURN_FALSE(retVal) \
    ZE_RESULT_SUCCESS_OR_RETURN_VALUE((retVal), false)

#define ZE_RESULT_SUCCESS_OR_RETURN(retVal)                                    \
    {                                                                          \
        const auto tempVarForDefine = (retVal);                                \
        ZE_RESULT_SUCCESS_OR_RETURN_VALUE(tempVarForDefine, tempVarForDefine); \
    }

#define ZE_RESULT_SUCCESS_OR_ERROR(retVal)                                                                                         \
    {                                                                                                                              \
        const auto tempVarForDefine = (retVal);                                                                                    \
        if (tempVarForDefine != ZE_RESULT_SUCCESS) {                                                                               \
            const auto _message = std::string("Fatal LevelZero error occurred, retVal=") + std::to_string(tempVarForDefine) + ")"; \
            ERROR(_message);                                                                                                       \
        }                                                                                                                          \
    }
