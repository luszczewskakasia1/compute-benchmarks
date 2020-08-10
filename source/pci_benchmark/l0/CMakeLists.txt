if(NOT TEST_L0)
    return()
endif()

file(GLOB_RECURSE SOURCES *.cpp *.h *.cl *.spv)
target_sources(${TARGET_NAME} PRIVATE ${SOURCES})
