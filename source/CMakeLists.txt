# Include helper functions
include(CMakeMacrosAddBenchmark.cmake)
include(CMakeMacrosAddFiles.cmake)

# Setup output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Add sources
set(SOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(framework)
add_subdirectory(ulls_benchmark)
add_subdirectory(memory_benchmark)
