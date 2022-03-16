#
# INTEL CONFIDENTIAL
#
# Copyright (C) 2022 Intel Corporation
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

set(SYCL_FOUND FALSE CACHE BOOL "SYCL Found")
set(INTEL_COMPILER_ROOT $ENV{CMPLR_ROOT})

if(NOT INTEL_COMPILER_ROOT)
    message(WARNING "FindSYCL: Intel oneAPI environment not set. Ensure that Intel oneAPI is installed and use the setvars script.")
    return()
endif()

set(_CXX_COMPILER ${CMAKE_CXX_COMPILER})
set(_CXX_COMPILER_ID ${CMAKE_CXX_COMPILER_ID})
set(CMAKE_CXX_COMPILER "dpcpp")
set(CMAKE_CXX_COMPILER_ID "IntelLLVM")

set(SYCL_INCLUDE_DIR ${INTEL_COMPILER_ROOT}/linux/include)
set(SYCL_LIBRARY_DIR ${INTEL_COMPILER_ROOT}/linux/lib)

find_package(IntelDPCPP) 
if(NOT ${IntelDPCPP_FOUND})
    message(WARNING "Intel DPCPP installation not found")
else()
    message(STATUS "Intel DPCPP installation found")
    string(REGEX REPLACE " $" "" SYCL_FLAGS "${SYCL_FLAGS}")
    
    set(SYCL_FOUND TRUE)
    set(SYCL_CFLAGS "${SYCL_FLAGS}" CACHE STRING "SYCL Compiler Flags")
    set(SYCL_LFLAGS "${SYCL_CFLAGS} -lsycl" CACHE STRING "SYCL Linker Flags")
    set(SYCL_INCLUDE_DIR "${SYCL_INCLUDE_DIR}" CACHE PATH "SYCL Inlcude Directory")

    set(SYCL_PROXY "${CMAKE_SOURCE_DIR}/scripts/dpcpp-proxy.sh" CACHE STRING "SYCL Compiler Proxy")
endif()

set(CMAKE_CXX_COMPILER ${_CXX_COMPILER})
set(CMAKE_CXX_COMPILER_ID ${_CXX_COMPILER_ID})
