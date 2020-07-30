if(NOT TEST_L0)
    return()
endif()

file(GLOB_RECURSE SOURCES *.cpp *.h *.cl *.spv)
target_sources(${TARGET_NAME} PRIVATE ${SOURCES})
set_property(TARGET ${TARGET_NAME} APPEND PROPERTY KERNELS
    "${CMAKE_CURRENT_SOURCE_DIR}/kernels/write_one.spv"
    "${CMAKE_CURRENT_SOURCE_DIR}/kernels/empty_kernel.spv"
)
