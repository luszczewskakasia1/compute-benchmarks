if(NOT TEST_L0)
    return()
endif()

file(GLOB_RECURSE SOURCES *.cpp *.h *.cl *.spv)
target_sources(${TARGET_NAME} PRIVATE ${SOURCES})

file(GLOB_RECURSE KERNELS *.spv)
set_property(TARGET ${TARGET_NAME} APPEND PROPERTY KERNELS ${KERNELS})
