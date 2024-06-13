set(LIB_NAME multiplexing)
add_library(${LIB_NAME} INTERFACE)
include_directories(inc/)
target_include_directories(${LIB_NAME} INTERFACE inc/)
