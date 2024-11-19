# FindANTLR4.cmake
# Locate the antlr4-runtime library

find_path(ANTLR4_INCLUDE_DIR antlr4-runtime.h
    PATH_SUFFIXES antlr4-runtime
)

find_library(ANTLR4_LIBRARY NAMES antlr4-runtime
    PATH_SUFFIXES lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ANTLR4 DEFAULT_MSG
    ANTLR4_INCLUDE_DIR ANTLR4_LIBRARY
)

mark_as_advanced(ANTLR4_INCLUDE_DIR ANTLR4_LIBRARY)
