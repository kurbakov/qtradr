# Turns on lots of compiler warning flags, specifically
# (-Waddress, -Wcomment, -Wformat, -Wbool-compare,
# -Wuninitialized, -Wunknown-pragmas, -Wunused-value, -Wunused-value …)
add_compile_options(-Wall)

# Turn any warning into a compilation error
add_compile_options(-Werror)

# Enables extra flags not enabled by -Wall, such as
# -Wsign-compare (C only), -Wtype-limits, -Wuninitialize
add_compile_options(-Wextra)

# Issue all warning required by ISO C and ISO C++ standard, it issues
# warning whenever there are compiler extensions non compliant to
# ISO C or C++ standard.
add_compile_options(-Wpedantic)
# add_compile_options(-Wconversion)
add_compile_options(-Wcast-align)
add_compile_options(-Wunused)
add_compile_options(-Wshadow)
# add_compile_options(-Wold-style-cast)
add_compile_options(-Wpointer-arith)
add_compile_options(-Wcast-qual)
add_compile_options(-Wno-missing-braces)
