aux_source_directory(. http_SRCS)

  
add_library(http ${http_SRCS})
target_link_libraries(http base net)

add_subdirectory(tests)