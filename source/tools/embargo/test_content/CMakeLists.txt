#
# INTEL CONFIDENTIAL
#
# Copyright (C) 2025 Intel Corporation
#
# This software and the related documents are Intel copyrighted materials,
# and your use of them is governed by the express license under which they were
# provided to you ("License"). Unless the License provides otherwise,
# you may not use, modify, copy, publish, distribute, disclose or transmit this
# software or the related documents without Intel's prior written permission.
#
# This software and the related documents are provided as is, with no express or
# implied warranties, other than those that are expressly stated in the License.

option(GENERATE_TEST_CONTENT "test content noop validation" ON)

if (NOT GENERATE_TEST_CONTENT)
    return()
endif()

find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(TARGET_NAME test_content_validation)

add_custom_target(${TARGET_NAME} ALL DEPENDS generate_commands.py)

# make sure test content validation is a last step
add_dependencies(${TARGET_NAME} run_docs_generator)

add_custom_command(
    TARGET ${TARGET_NAME}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    POST_BUILD
    COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/generate_commands.py" 
            --output_path "${CMAKE_CURRENT_SOURCE_DIR}" 
            --binaries_path ${OUTPUT_DIR}
            --limited
    VERBATIM
)

add_custom_command(
    TARGET ${TARGET_NAME}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    POST_BUILD
    COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/generate_commands.py" 
            --output_path "${CMAKE_CURRENT_SOURCE_DIR}" 
            --binaries_path ${OUTPUT_DIR}
    VERBATIM
)
