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

if (NOT GENERATE_DOCS)
    return()
endif()

# Prepare targets list
get_property(TARGETS_FOR_DOCS_GENERATION GLOBAL PROPERTY TARGETS_FOR_DOCS_GENERATION)
set(TARGETS "")
foreach(TGT ${TARGETS_FOR_DOCS_GENERATION})
    string(APPEND TARGETS "{\"${TGT}\", \"$<TARGET_FILE:${TGT}>\"}, ")
endforeach()

# Setup executable target
set(TARGET_NAME docs_generator)
file(GLOB SOURCES *.cpp *.h)
add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME} PRIVATE compute_benchmarks_framework)
target_compile_definitions(${TARGET_NAME} PRIVATE TARGETS=${TARGETS})
add_dependencies(${TARGET_NAME} ${TARGETS_FOR_DOCS_GENERATION})
setup_vs_folders(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR})

# Setup custom target to run generation on build
set(RUNNER_TARGET_NAME "run_${TARGET_NAME}")
add_custom_target(${RUNNER_TARGET_NAME}
    ALL
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/TESTS.md
)
add_dependencies(${RUNNER_TARGET_NAME} ${TARGET_NAME})
add_custom_command(
    TARGET ${RUNNER_TARGET_NAME}
    WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>"
    POST_BUILD
    COMMAND "$<TARGET_FILE:${TARGET_NAME}>" --path=${CMAKE_SOURCE_DIR}/TESTS.md
)
