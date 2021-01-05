function(copy_kernels_to_bin_directory TARGET_NAME)
    get_target_property(KERNELS ${TARGET_NAME} KERNELS)
    if (KERNELS STREQUAL "KERNELS-NOTFOUND")
        return()
    endif()
    foreach(KERNEL ${KERNELS})
        get_filename_component(KERNEL_NAME ${KERNEL} NAME)
        add_custom_command(
            TARGET ${TARGET_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${KERNEL}
            $<TARGET_FILE_DIR:${TARGET_NAME}>/${KERNEL_NAME}
        )
    endforeach()
endfunction()

function(setup_vs_folders TARGET_NAME BASE_DIR)
    if (MSVC)
        get_target_property(SOURCES ${TARGET_NAME} SOURCES)
        source_group (TREE ${BASE_DIR} FILES ${SOURCES})
    endif()
endfunction()

function(add_subdirectories)
  file(GLOB SUB_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
  foreach(SUB_DIR ${SUB_DIRS})
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${SUB_DIR}/CMakeLists.txt)
      add_subdirectory(${SUB_DIR})
    endif()
  endforeach()
endfunction()

function (add_benchmark BASE_TARGET_NAME)
    foreach(API ${ARGN})
        if (${API} STREQUAL "all")
            if (BUILD_ALL_API_BINARIES)
                get_property(ALL_APIS GLOBAL PROPERTY APIS)
                add_benchmark_for_api(${BASE_TARGET_NAME} OFF "${ALL_APIS}")
            endif()
        else()
            if (BUILD_SINGLE_API_BINARIES)
                add_benchmark_for_api(${BASE_TARGET_NAME} ON ${API})
            endif()
        endif()
    endforeach()
endfunction()

function (add_benchmark_for_api BASE_TARGET_NAME APPEND_API_TO_TARGET_NAME APIS)
    # Get names
    set(TARGET_NAME "${BASE_TARGET_NAME}")
    set(TARGET_FOLDER_NAME "tests")
    if (APPEND_API_TO_TARGET_NAME)
        string(APPEND TARGET_NAME "_${APIS}")
        string(APPEND TARGET_FOLDER_NAME "_${APIS}")
    endif()

    # Define target
    add_executable(${TARGET_NAME} CMakeLists.txt)
    target_link_libraries(${TARGET_NAME} PRIVATE compute_benchmarks_framework ${APIS})
    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER ${TARGET_FOLDER_NAME})
    if (BUILD_FOR_PUBLISHING)
        set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BASE_TARGET_NAME})
    endif()
    if (BUILD_FOR_PUBLISHING OR INCLUDE_VERSION)
        target_compile_definitions(${TARGET_NAME} PUBLIC BENCHMARK_VERSION="${BENCHMARK_VERSION}")
    else()
        target_compile_definitions(${TARGET_NAME} PUBLIC BENCHMARK_VERSION="")
    endif()

    # API agnostic sources
    set(API_AGNOSTIC_SOURCE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_SOURCE_DIR}/gtest
        ${CMAKE_CURRENT_SOURCE_DIR}/definitions
    )
    foreach(DIR ${API_AGNOSTIC_SOURCE_DIRECTORIES})
        add_sources_to_benchmark(${TARGET_NAME} ${DIR})
    endforeach()
    add_kernels_to_benchmark(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/kernels)

    # API specific sources
    set(API_SPECIFIC_SOURCE_DIRECTORIES
        ${SOURCE_ROOT}/common
        ${CMAKE_CURRENT_SOURCE_DIR}/implementations
    )
    foreach(API ${APIS})
        foreach(PARENT_DIR ${API_SPECIFIC_SOURCE_DIRECTORIES})
            set(DIR ${PARENT_DIR}/${API})
            add_sources_to_benchmark(${TARGET_NAME} ${DIR})
        endforeach()
    endforeach()

    # Ensure kernels reside inside benchmark's working directory
    copy_kernels_to_bin_directory(${TARGET_NAME})

    # Create directory structure in Visual Studio
    setup_vs_folders(${TARGET_NAME} ${SOURCE_ROOT})
endfunction()
