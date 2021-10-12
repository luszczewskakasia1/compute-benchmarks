#
# INTEL CONFIDENTIAL
#
# Copyright (C) 2019-2021 Intel Corporation
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

include(FindPackageHandleStandardArgs)

find_path(LevelZero_INCLUDE_DIR
  NAMES level_zero/ze_api.h
  PATHS
    ${L0_ROOT}
  PATH_SUFFIXES "include"
)

find_library(LevelZero_LIBRARY
  NAMES ze_loader ze_loader32 ze_loader64
  PATHS
    ${L0_ROOT}
  PATH_SUFFIXES "lib" "lib/"
)

find_package_handle_standard_args(LevelZero
  REQUIRED_VARS
    LevelZero_INCLUDE_DIR
    LevelZero_LIBRARY
  HANDLE_COMPONENTS
)
mark_as_advanced(LevelZero_LIBRARY LevelZero_INCLUDE_DIR)

if(LevelZero_FOUND)
    list(APPEND LevelZero_LIBRARIES ${LevelZero_LIBRARY} ${CMAKE_DL_LIBS})
    list(APPEND LevelZero_INCLUDE_DIRS ${LevelZero_INCLUDE_DIR})
endif()
