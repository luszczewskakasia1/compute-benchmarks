# Get libraries and headers for the SDK
find_package(OpenCL QUIET)
if(NOT ${OpenCL_FOUND})
    set(ADDITIONAL_PATH "${CMAKE_SOURCE_DIR}/third_party/opencl-sdk")
    message(STATUS "OpenCL installation not found. Using ${ADDITIONAL_PATH}")

    if (WIN32)
        set(LIB_NAME OpenCL.lib)
    else()
        set(LIB_NAME libOpenCL.so)
    endif()
    set(OpenCL_LIBRARIES    "${ADDITIONAL_PATH}/lib/x64/${LIB_NAME}")
    set(OpenCL_INCLUDE_DIRS "${ADDITIONAL_PATH}/include")
else()
    message(STATUS "OpenCL installation found")
endif()

# Get sources
file(GLOB_RECURSE SOURCES *.cpp *.h)

# Define target
set(API_NAME ocl)
set(TARGET_NAME compute_benchmarks_framework_${API_NAME})
add_library(${TARGET_NAME} STATIC ${SOURCES})
target_link_libraries(${TARGET_NAME} PUBLIC ${OpenCL_LIBRARIES})
target_link_libraries(${TARGET_NAME} PUBLIC compute_benchmarks_framework)
target_include_directories(${TARGET_NAME} PUBLIC ${OpenCL_INCLUDE_DIRS})
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/third_party/opencl-intel)
if (MSVC)
    set_target_properties(${TARGET_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Add this API to global array
set_property(GLOBAL APPEND PROPERTY APIS ${API_NAME})
