add_library(multiplexing STATIC
    src/EPoll.cpp
)

target_include_directories(concurrency PUBLIC inc/)

include_directories(
    inc/
)
