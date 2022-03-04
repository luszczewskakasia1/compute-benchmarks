#
# INTEL CONFIDENTIAL
#
# Copyright (C) 2019-2022 Intel Corporation
#
# This software and the related documents are Intel copyrighted materials,
# and your use of them is governed by the express license under which they were
# provided to you ("License"). Unless the License provides otherwise,
# you may not use, modify, copy, publish, distribute, disclose or transmit this
# software or the related documents without Intel's prior written permission.
#
# This software and the related documents are provided as is, with no express or
# implied warranties, other than those that are expressly stated in the License.
#

if (NOT BUILD_SYCL)
    return()
endif()

# Get libraries and headers for the SDK
find_package(IntelDPCPP QUIET) 
if(NOT ${IntelDPCPP_FOUND})
    message(WARNING "Intel DPCPP installation not found - SYCL benchmarks will not be built")
else()
    message(STATUS "Intel DPCPP installation found")

    # Get sources
    file(GLOB_RECURSE SOURCES *.cpp *.h)

    # Define target
    set(API_NAME sycl)
    set(TARGET_NAME compute_benchmarks_framework_${API_NAME})
    add_library(${TARGET_NAME} STATIC ${SOURCES})
    target_link_libraries(${TARGET_NAME} PUBLIC -lsycl)
    target_link_libraries(${TARGET_NAME} PUBLIC compute_benchmarks_framework)
    target_include_directories(${TARGET_NAME} PUBLIC ${SYCL_INCLUDE_DIR} ${SYCL_INCLUDE_DIR}/sycl)
    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER framework)
    add_subdirectories()
    setup_vs_folders(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
    setup_warning_options(${TARGET_NAME})
    setup_output_directory(${TARGET_NAME})
    if (MSVC)
        set_target_properties(${TARGET_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
    endif()

    # Add this API to global array
    set_property(GLOBAL APPEND PROPERTY APIS ${API_NAME})
endif()
