# Helper functions
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

function (add_benchmark TARGET_NAME)
    add_executable(${TARGET_NAME} CMakeLists.txt)
    link_to_framework(${TARGET_NAME} compute_benchmarks_framework)

    # Add sources
    file(GLOB SOURCES *.cpp *.h)
    target_sources(${TARGET_NAME} PRIVATE ${SOURCES})
    add_subdirectories()

    # Ensure kernels reside inside benchmark's working directory
    copy_kernels_to_bin_directory(${TARGET_NAME})

    # Create directory structure in Visual Studio
    setup_vs_folders(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
endfunction()


# Setup output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Add sources
set(SOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(framework)
add_subdirectory(ulls_benchmark)
add_subdirectory(pci_benchmark)
