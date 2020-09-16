file(GLOB_RECURSE SOURCES *.cpp *.h *inl)
target_sources(${TARGET_NAME} PRIVATE ${SOURCES})
