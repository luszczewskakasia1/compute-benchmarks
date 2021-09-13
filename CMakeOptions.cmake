macro(benchmark_option NAME DEFAULT_VALUE)
    if (NOT DEFINED ${NAME})
        set(${NAME} ${DEFAULT_VALUE})
    endif()
    message(STATUS "      ${NAME}=${${NAME}} ${ARGN}")
endmacro()
macro (benchmark_option_group DESCRIPTION)
    message(STATUS "   ${DESCRIPTION}:")
endmacro()
message(STATUS "Build options:")

# Selecting targets
benchmark_option_group("Flags for selecting targets to be built")
benchmark_option(BUILD_L0 ON)
benchmark_option(BUILD_OCL ON)
if (NOT BUILD_L0 OR NOT BUILD_OCL)
    set(BUILD_ALL_API_BINARIES OFF)
    set(BUILD_ALL_API_BINARIES_COMMENT "(Disabled due to not all APIs being enabled)")
endif()
benchmark_option(BUILD_SINGLE_API_BINARIES ON)
benchmark_option(BUILD_ALL_API_BINARIES OFF ${BUILD_ALL_API_BINARIES_COMMENT})

# Publish builds and versioning
benchmark_option_group("Flags for configuring publishable builds and versions")
benchmark_option(BUILD_FOR_PUBLISHING OFF)
if (BUILD_FOR_PUBLISHING)
    set(INCLUDE_VERSION_COMMENT "(Enabled by BUILD_FOR_PUBLISHING)")
endif()
benchmark_option(INCLUDE_VERSION OFF ${INCLUDE_VERSION_COMMENT})
benchmark_option(VERSION_INDEX "01")

# Miscellaneous flags
benchmark_option_group("Miscellaneous flags")
benchmark_option(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin")
benchmark_option(BUILD_HELLO_WORLD OFF)
benchmark_option(GENERATE_DOCS ON)
benchmark_option(BUILD_TOOLS ON)
benchmark_option(LOG_BENCHMARK_TARGETS OFF)

# Additional checks
if (NOT BUILD_L0 AND NOT BUILD_OCL)
    message(FATAL_ERROR "No API was selected for testing. No benchmarks will be produced")
endif()
