# Define Framework library
file(GLOB SOURCES *.cpp *.h)
set(TARGET_NAME compute_benchmarks_framework)
add_library(${TARGET_NAME} SHARED ${SOURCES})
target_link_libraries(${TARGET_NAME} PUBLIC gtest)
target_include_directories(${TARGET_NAME} PUBLIC ${SOURCE_ROOT})
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_17)
add_subdirectories()

# Ignore CRT Warnings
if (MSVC)
    target_compile_definitions(${TARGET_NAME} PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()

# Export all symbols on VS builds (so __dllexport is not required)
if (MSVC)
    set_target_properties(${TARGET_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Link to PciAccess
if (USE_PCIACCESS)
    message(STATUS "Linking to libpciaccess.so")
    set(PCIACCESS_ROOT "${CMAKE_SOURCE_DIR}/third_party/pciaccess")
    target_link_libraries(${TARGET_NAME} PUBLIC "${PCIACCESS_ROOT}/lib/x64/libpciaccess.so.0")
    target_include_directories(${TARGET_NAME} PUBLIC "${PCIACCESS_ROOT}/include")
    target_compile_definitions(${TARGET_NAME} PUBLIC USE_PCIACCESS)
endif()

# Create directory structure in Visual Studio
setup_vs_folders(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
