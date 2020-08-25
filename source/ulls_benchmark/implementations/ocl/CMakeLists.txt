if(NOT TEST_OCL)
    return()
endif()
add_sources_and_kernels_to_benchmark(${TARGET_NAME})
