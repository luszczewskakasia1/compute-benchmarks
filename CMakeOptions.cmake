# Option for output dir
if (NOT OUTPUT_DIR)
    set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin")
endif()
message(STATUS "Binary output directory: ${OUTPUT_DIR}")

# Option for OCL
option(TEST_OCL "Enable OpenCL benchmarks" ON)
if(NOT TEST_OCL)
    message(STATUS "Skipping OpenCL tests")
else()
    message(STATUS "Including OpenCL tests")
endif()

# Option for L0
option(TEST_L0 "Enable LevelZero benchmarks" ON)
if(NOT TEST_L0)
    message(STATUS "Skipping LevelZero tests")
else()
    message(STATUS "Including LevelZero tests")
endif()
