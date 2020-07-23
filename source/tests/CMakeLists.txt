file(GLOB SOURCES *.cpp *.h)
target_sources(${TARGET_NAME} PRIVATE ${SOURCES})

add_subdirectory(l0)
add_subdirectory(ocl)
