option(TEST_L0 "Enable LevelZero benchmarks" ON)
if(NOT TEST_L0)
    message(STATUS "Skipping LevelZero tests")
    return()
endif()
message(STATUS "Including LevelZero tests")

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

file(GLOB_RECURSE SOURCES *.cpp *.h *.cl *.spv)
target_sources(${TARGET_NAME} PRIVATE ${SOURCES})

add_library(LevelZeroInterface INTERFACE)
target_link_libraries(LevelZeroInterface INTERFACE ${LevelZero_LIBRARIES})
target_include_directories(LevelZeroInterface INTERFACE ${LevelZero_INCLUDE_DIRS})
target_include_directories(LevelZeroInterface INTERFACE ${CMAKE_SOURCE_DIR}/third_party/experimental_level_zero)
set_property(TARGET ${TARGET_NAME} APPEND PROPERTY KERNELS
    "${CMAKE_CURRENT_SOURCE_DIR}/Kernels/write.spv"
    "${CMAKE_CURRENT_SOURCE_DIR}/Kernels/empty.spv"
)
