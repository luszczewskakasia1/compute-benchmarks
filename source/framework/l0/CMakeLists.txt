find_package(LevelZero QUIET)
if(NOT ${LevelZero_FOUND})
    set(ADDITIONAL_PATH "${CMAKE_SOURCE_DIR}/third_party/level-zero-sdk")
    message(STATUS "Level Zero installation not found. Trying ${ADDITIONAL_PATH}")

    if (WIN32)
        set(LIB_NAME ze_loader.lib)
    else()
        set(LIB_NAME libze_loader.so)
    endif()
    set(LevelZero_LIBRARIES    "${ADDITIONAL_PATH}/lib/x64/${LIB_NAME}")
    set(LevelZero_INCLUDE_DIRS "${ADDITIONAL_PATH}/include")
else()
    message(STATUS "Level Zero installation found")
endif()

# Add helper files and headers to the framework
file(GLOB_RECURSE SOURCES *.cpp *.h)
target_sources(${TARGET_NAME} PRIVATE ${SOURCES})
target_include_directories(${TARGET_NAME} PRIVATE ${LevelZero_INCLUDE_DIRS})
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/third_party/level-zero-intel)

# Library used by benchmarks if they want to use LevelZero
set(API_NAME l0)
add_library(${API_NAME} INTERFACE)
target_link_libraries(${API_NAME} INTERFACE ${LevelZero_LIBRARIES})
target_include_directories(${API_NAME} INTERFACE ${LevelZero_INCLUDE_DIRS})
target_include_directories(${API_NAME} INTERFACE ${CMAKE_SOURCE_DIR}/third_party/level-zero-intel)
set_property(GLOBAL APPEND PROPERTY APIS ${API_NAME})
