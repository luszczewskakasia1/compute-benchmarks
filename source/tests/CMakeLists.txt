set(TARGET_NAME ulls_benchmarks)
add_executable(${TARGET_NAME} CMakeLists.txt)
target_link_libraries(${TARGET_NAME} PRIVATE compute_benchmarks_framework)
set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")

# Add sources
file(GLOB SOURCES *.cpp *.h)
target_sources(${TARGET_NAME} PRIVATE ${SOURCES})
add_subdirectory(gtest)
add_subdirectory(l0)
add_subdirectory(ocl)

# Ensure kernels reside inside benchmark's working directory
copy_kernels_to_bin_directory(${TARGET_NAME})
