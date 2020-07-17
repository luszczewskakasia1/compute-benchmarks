# Find OpenCL
find_package(OpenCL QUIET)
if(NOT ${OpenCL_FOUND})
    set(ADDITIONAL_PATH "${CMAKE_SOURCE_DIR}/opencl-sdk")
    message(STATUS "OpenCL installation not found. Trying ${ADDITIONAL_PATH}")

    set(OpenCL_LIBRARIES    "${ADDITIONAL_PATH}/lib/x64/OpenCL.lib")
    set(OpenCL_INCLUDE_DIRS "${ADDITIONAL_PATH}/include")

    if(NOT EXISTS ${OpenCL_LIBRARIES} OR NOT EXISTS ${OpenCL_INCLUDE_DIRS})
        message(FATAL_ERROR "OpenCL not found.")
    endif()
endif()

# Find LevelZero
find_package(LevelZero QUIET)
if(NOT ${LevelZero_FOUND})
    set(ADDITIONAL_PATH "${CMAKE_SOURCE_DIR}/level-zero-sdk")
    message(STATUS "Level Zero installation not found. Trying ${ADDITIONAL_PATH}")

    set(LevelZero_LIBRARIES    "${ADDITIONAL_PATH}/lib/x64/ze_loader.lib")
    set(LevelZero_INCLUDE_DIRS "${ADDITIONAL_PATH}/include")

    if(NOT EXISTS ${LevelZero_LIBRARIES} OR NOT EXISTS ${LevelZero_INCLUDE_DIRS})
        message(FATAL_ERROR "LevelZero not found.")
    endif()
endif()

# Define target
set(TARGET_NAME ulls_benchmark)
file(GLOB_RECURSE SOURCES *.cpp *.h)
source_group (TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME} PRIVATE ${OpenCL_LIBRARIES} ${LevelZero_LIBRARIES})
target_link_libraries(${TARGET_NAME} PRIVATE gtest)
target_include_directories(${TARGET_NAME} PRIVATE ${OpenCL_INCLUDE_DIRS} ${LevelZero_INCLUDE_DIRS})
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${TARGET_NAME} PRIVATE ../third_party/googletest/googletest/include)
if(DEFINED OUTPUT_DIR)
    set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
endif()
