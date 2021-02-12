file(GLOB_RECURSE SOURCES *.cpp *.h)
target_sources(${TARGET_NAME} PRIVATE ${SOURCES})