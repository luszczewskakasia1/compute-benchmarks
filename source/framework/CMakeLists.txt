# Define Framework library
file(GLOB SOURCES *.cpp *.h)
set(TARGET_NAME compute_benchmarks_framework)
add_library(${TARGET_NAME} STATIC ${SOURCES})
target_link_libraries(${TARGET_NAME} PUBLIC gtest)
target_include_directories(${TARGET_NAME} PUBLIC ${SOURCE_ROOT})
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_17)

# Add subdirectories
add_subdirectory(test_case)

# Ignore CRT Warnings
if (MSVC)
    target_compile_definitions(${TARGET_NAME} PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()

# Link to compute libraries
add_subdirectory(ocl)
add_subdirectory(l0)
function(tryLinkTo DEPENDENCY)
    if(TARGET ${DEPENDENCY})
        target_link_libraries(${TARGET_NAME} PUBLIC ${DEPENDENCY})
    endif()
endfunction()
tryLinkTo(OclInterface)
tryLinkTo(LevelZeroInterface)

# Create directory structure in Visual Studio
setup_vs_folders(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
