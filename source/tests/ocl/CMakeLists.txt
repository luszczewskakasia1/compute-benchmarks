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

file(GLOB_RECURSE SOURCES *.cpp *.h)
target_sources(${TARGET_NAME} PRIVATE ${SOURCES})
target_link_libraries(${TARGET_NAME} PRIVATE ${OpenCL_LIBRARIES})
target_include_directories(${TARGET_NAME} PRIVATE ${OpenCL_INCLUDE_DIRS})
