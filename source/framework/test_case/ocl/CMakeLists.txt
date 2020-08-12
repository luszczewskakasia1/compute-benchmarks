if(NOT TEST_OCL)
    return()
endif()

file(GLOB SOURCES *.cpp *.h)
target_sources(${TARGET_NAME} PRIVATE ${SOURCES})
