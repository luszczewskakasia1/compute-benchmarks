macro(benchmark_option NAME DEFAULT_VALUE)
    if (NOT DEFINED ${NAME})
        set(${NAME} ${DEFAULT_VALUE})
    endif()
    message(STATUS "  ${NAME}=${${NAME}}")
endmacro()

message(STATUS "Build options:")
benchmark_option(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin")
