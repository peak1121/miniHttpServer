aux_source_directory(. base_SRCS)
add_library(base ${base_SRCS})
target_link_libraries(base pthread rt)

add_subdirectory(tests)
