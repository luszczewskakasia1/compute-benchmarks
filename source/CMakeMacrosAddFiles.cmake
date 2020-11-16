function(add_sources_to_benchmark TARGET_NAME DIR)
    file(GLOB SOURCES ${DIR}/*.cpp ${DIR}/*.h)
    target_sources(${TARGET_NAME} PRIVATE ${SOURCES})
endfunction()

function (add_kernels_to_benchmark TARGET_NAME DIR)
    file(GLOB_RECURSE SOURCE_KERNELS ${DIR}/*.cl)
    target_sources(${TARGET_NAME} PRIVATE ${SOURCE_KERNELS})

    file(GLOB KERNELS ${DIR}/*.spv ${DIR}/copyable_sources/*.cl)
    set_property(TARGET ${TARGET_NAME} APPEND PROPERTY KERNELS ${KERNELS})
endfunction()
