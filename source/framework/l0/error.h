#pragma once

#include "framework/utility/error.h"

#include <gtest/gtest.h>
#include <string>

#define ASSERT_ZE_RESULT_SUCCESS(retVal)      \
    if (retVal != ZE_RESULT_SUCCESS) {        \
        EXPECT_EQ(ZE_RESULT_SUCCESS, retVal); \
        return TestResult::Error;             \
    }

#define EXPECT_ZE_RESULT_SUCCESS(retVal)      \
    if (retVal != ZE_RESULT_SUCCESS) {        \
        EXPECT_EQ(ZE_RESULT_SUCCESS, retVal); \
    }

#define ZE_RESULT_SUCCESS_OR_TERMINATE(retVal)                                                  \
    if (retVal != ZE_RESULT_SUCCESS) {                                                          \
        ERROR(std::string("Fatal LevelZero error occurred, retVal=") + std::to_string(retVal)); \
    }
