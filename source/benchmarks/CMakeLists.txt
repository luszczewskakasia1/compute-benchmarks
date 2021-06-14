include(CMakeMacrosAddBenchmark.cmake)

set(BENCHMARKS_SOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(atomic_benchmark)
add_subdirectory(api_overhead_benchmark)
add_subdirectory(gpu_cmds_benchmark)
add_subdirectory(memory_benchmark)
add_subdirectory(miscellaneous_benchmark)
add_subdirectory(multitile_memory_benchmark)
add_subdirectory(overlap_benchmark)
add_subdirectory(ulls_benchmark)


if (GENERATE_DOCS)
    set(TARGET_NAME generate_docs)
    add_custom_target(${TARGET_NAME})

    message(STATUS "Setting up target for docs generation. To generate docs, build ${TARGET_NAME}. Targets used for docs generation:")
    get_property(TARGETS_FOR_DOCS_GENERATION GLOBAL PROPERTY TARGETS_FOR_DOCS_GENERATION)
    foreach(TARGET ${TARGETS_FOR_DOCS_GENERATION})
        message(STATUS "    ${TARGET}")
    endforeach()

    add_custom_command(
        TARGET ${TARGET_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/TESTS.md
    )
    foreach(TARGET ${TARGETS_FOR_DOCS_GENERATION})
        add_dependencies(${TARGET_NAME} ${TARGET})
        add_custom_command(
            TARGET ${TARGET_NAME}
            WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET}>"
            POST_BUILD
            COMMAND "$<TARGET_FILE:${TARGET}>" --generateDocs --generateDocsPath=${CMAKE_SOURCE_DIR}/TESTS.md
        )
    endforeach()
endif()
