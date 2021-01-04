#pragma once

#include "framework/utility/error.h"

#include <gtest/gtest.h>
#include <string>

#define ASSERT_CL_SUCCESS(retVal)      \
    if (retVal != CL_SUCCESS) {        \
        EXPECT_EQ(CL_SUCCESS, retVal); \
        return TestResult::Error;      \
    }

#define EXPECT_CL_SUCCESS(retVal)      \
    if (retVal != CL_SUCCESS) {        \
        EXPECT_EQ(CL_SUCCESS, retVal); \
    }

#define CL_SUCCESS_OR_RETURN_VALUE(retVal, value) \
    if ((retVal) != CL_SUCCESS) {                 \
        return (value);                           \
    }

#define CL_SUCCESS_OR_RETURN_FALSE(retVal) CL_SUCCESS_OR_RETURN_VALUE((retVal), false)

#define ERROR_UNLESS_CL_SUCCESS(retVal, message)                                                      \
    {                                                                                                 \
        const auto _retVal = retVal;                                                                  \
        if (_retVal != CL_SUCCESS) {                                                                  \
            const auto _message = std::string(message) + " (retVal=" + std::to_string(_retVal) + ")"; \
            ERROR(_message);                                                                          \
        }                                                                                             \
    }
