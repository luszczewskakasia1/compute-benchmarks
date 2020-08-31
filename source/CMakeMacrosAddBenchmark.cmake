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

function(link_to_framework TARGET_NAME LIBRARY_NAME)
    # The framework uses static initializers to register device info print functions.
    # Those tend to be optimized away by the compilers, so we need to pass compiler
    # specific linker flags to avoid this. Various compilers are explicitly supported.
    # The 'else' branch will still produce a functional benchmark, but the static
    # initializers MAY be optimized away and no device info may be printed.

    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_link_libraries(${TARGET_NAME} PRIVATE ${LIBRARY_NAME})
        set_property(TARGET ${TARGET_NAME} APPEND PROPERTY LINK_FLAGS /WHOLEARCHIVE:${LIBRARY_NAME})
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_libraries(${TARGET_NAME} PRIVATE
          "-Wl,--whole-archive"
          ${LIBRARY_NAME}
          "-Wl,--no-whole-archive"
        )
    else()
        target_link_libraries(${TARGET_NAME} PRIVATE ${LIBRARY_NAME})
        message(WARNING "Device info might not be printed")
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
    get_property(APIS GLOBAL PROPERTY APIS)
    add_benchmark_for_api(${BASE_TARGET_NAME} OFF "${APIS}")
    foreach(API ${APIS})
        add_benchmark_for_api(${BASE_TARGET_NAME} ON ${API})
    endforeach()
endfunction()

function (add_benchmark_for_api BASE_TARGET_NAME APPEND_API_TO_TARGET_NAME APIS)
    # Get target name
    if (APPEND_API_TO_TARGET_NAME)
        set(TARGET_NAME "${BASE_TARGET_NAME}_${APIS}")
    else()
        set(TARGET_NAME "${BASE_TARGET_NAME}")
    endif()

    # Define target
    add_executable(${TARGET_NAME} CMakeLists.txt)
    link_to_framework(${TARGET_NAME} compute_benchmarks_framework)
    target_link_libraries(${TARGET_NAME} PRIVATE ${APIS})

    # API agnostic sources
    set(API_AGNOSTIC_SOURCE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/gtest
        ${CMAKE_CURRENT_SOURCE_DIR}/definitions
    )
    foreach(DIR ${API_AGNOSTIC_SOURCE_DIRECTORIES})
        add_sources_to_benchmark(${TARGET_NAME} ${DIR})
    endforeach()

    # API specific sources
    set(API_SPECIFIC_SOURCE_DIRECTORIES
        ${SOURCE_ROOT}/common
        ${CMAKE_CURRENT_SOURCE_DIR}/implementations
    )
    foreach(API ${APIS})
        foreach(PARENT_DIR ${API_SPECIFIC_SOURCE_DIRECTORIES})
            set(DIR ${PARENT_DIR}/${API})
            add_sources_to_benchmark(${TARGET_NAME} ${DIR})
            add_kernels_to_benchmark(${TARGET_NAME} ${DIR}/Kernels)
        endforeach()
    endforeach()

    # Ensure kernels reside inside benchmark's working directory
    copy_kernels_to_bin_directory(${TARGET_NAME})

    # Create directory structure in Visual Studio
    setup_vs_folders(${TARGET_NAME} ${SOURCE_ROOT})
endfunction()
