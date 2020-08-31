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

# Add helper files and headers to the framework
file(GLOB_RECURSE SOURCES *.cpp *.h)
target_sources(${TARGET_NAME} PRIVATE ${SOURCES})
target_include_directories(${TARGET_NAME} PRIVATE ${OpenCL_INCLUDE_DIRS})
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/third_party/opencl-intel)

# Library used by benchmarks if they want to use OpenCL
set(API_NAME ocl)
add_library(${API_NAME} INTERFACE)
target_link_libraries(${API_NAME} INTERFACE ${OpenCL_LIBRARIES})
target_include_directories(${API_NAME} INTERFACE ${OpenCL_INCLUDE_DIRS})
target_include_directories(${API_NAME} INTERFACE ${CMAKE_SOURCE_DIR}/third_party/opencl-intel)
set_property(GLOBAL APPEND PROPERTY APIS ${API_NAME})
