aux_source_directory(. net_SRCS)

  
add_library(net ${net_SRCS})
target_link_libraries(net base pthread rt)

add_subdirectory(tests)