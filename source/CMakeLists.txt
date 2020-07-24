# Define executable
add_executable(${TARGET_NAME} CMakeLists.txt)
target_link_libraries(${TARGET_NAME} PRIVATE gtest)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${TARGET_NAME} PRIVATE ../third_party/googletest/googletest/include)
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")

# Add sources
add_subdirectory(framework)
add_subdirectory(tests)

# Setup VS folders
if (MSVC)
    get_target_property(SOURCES ${TARGET_NAME} SOURCES)
    source_group (TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
endif()

# Link to compute libraries
function(tryLinkTo DEPENDENCY)
    if(TARGET ${DEPENDENCY})
        target_link_libraries(${TARGET_NAME} PRIVATE ${DEPENDENCY})
    endif()
endfunction()
tryLinkTo(OclInterface)
tryLinkTo(LevelZeroInterface)

# Copy kernels to bin directory
get_target_property(KERNELS ${TARGET_NAME} KERNELS)
foreach(KERNEL ${KERNELS})
    get_filename_component(KERNEL_NAME ${KERNEL} NAME)
    add_custom_command(
        TARGET ${TARGET_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${KERNEL}
        ${OUTPUT_DIR}/$<IF:$<CONFIG:Debug>,Debug,Release>/${KERNEL_NAME}
    )
endforeach()
