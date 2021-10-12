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

#define NOMINMAX
#include <Windows.h>

inline std::string getErrorFromLastErrorCode() {
    const DWORD lastErrorCode = GetLastError();
    const DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                        FORMAT_MESSAGE_IGNORE_INSERTS;

    char *buffer = {};
    const DWORD bufferSize = FormatMessageA(flags,
                                            NULL,
                                            lastErrorCode,
                                            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                            reinterpret_cast<LPTSTR>(&buffer),
                                            0, NULL);
    if (bufferSize == 0) {
        return "unknown error";
    }

    std::string result{buffer};

    if (LocalFree(buffer) != nullptr) {
        result += " (could not deallocate error message buffer)";
    }

    return result;
}

#define FATAL_ERROR_IF_SYS_CALL_FAILED(call, message) FATAL_ERROR_IF((call) == 0, std::string(message) + ", " + getErrorFromLastErrorCode());
