# Try to find Part4 library
# Once done this will define
# CPPLIB_FOUND - if system found Part4 library
# CPPLIB_INCLUDE_DIRS - the Part4 include directories
# CPPLIB_LIBRARIES - the libraries needed to use Part4
# CPPLIB_DEFINITIONS - compiler switches required for using Part4

set (CPPLIB_ROOT_DIR $ENV{GIT_REPO}/cpplib)
# Uncomment the following line to print which directory CMake is looking in.
# MESSAGE(STATUS "CPPLIB_ROOT_DIR: " ${CPPLIB_ROOT_DIR})

# ppk_assert
find_library(LIB_ASSERT
    NAMES ppk_assert
    PATHS ${CPPLIB_ROOT_DIR}/lib
    DOC "The ppk_assert library"
)

# strutils
find_library(LIB_STRUTILS
    NAMES strutils
    PATHS ${CPPLIB_ROOT_DIR}/lib
    DOC "The strutils library"
)

# osutils
find_library(LIB_OSUTILS
    NAMES osutils
    PATHS ${CPPLIB_ROOT_DIR}/lib
    DOC "The osutils library"
)

# ioutils
find_library(LIB_IOUTILS
    NAMES ioutils
    PATHS ${CPPLIB_ROOT_DIR}/lib
    DOC "The ioutils library"
)

# gzipio
find_library(LIB_GZIPIO
    NAMES gzipio
    PATHS ${CPPLIB_ROOT_DIR}/lib
    DOC "The gzipio library"
)

# lz4io
find_library(LIB_LZ4IO
    NAMES lz4io
    PATHS ${CPPLIB_ROOT_DIR}/lib
    DOC "The lz4io library"
)

# argsparser
find_library(LIB_ARGSPARSER
    NAMES argsparser
    PATHS ${CPPLIB_ROOT_DIR}/lib
    DOC "The argsparser library"
)

# graph
find_library(LIB_GRAPH
    NAMES graph
    PATHS ${CPPLIB_ROOT_DIR}/lib
    DOC "The STL version graph library"
)

# math
find_library(LIB_MATH
    NAMES mathutils
    PATHS ${CPPLIB_ROOT_DIR}/lib
    DOC "The STL version graph library"
)

# rngutils
find_library(LIB_RNGUTILS
    NAMES rngutils
    PATHS ${CPPLIB_ROOT_DIR}/lib
    DOC "The random number library"
)

# hll
find_library(LIB_HLL
    NAMES hll
    PATHS ${CPPLIB_ROOT_DIR}/lib
    DOC "The HyperLogLog counting library"
)


# handle the QUIETLY and REQUIRED arguments and set
# CPPLIB_FOUND to TRUE if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  CPPLIB
  DEFAULT_MSG
  LIB_STRUTILS
  LIB_IOUTILS
  LIB_LZ4IO
  LIB_GZIPIO
  LIB_ASSERT
  LIB_OSUTILS
  LIB_ARGSPARSER
  LIB_RNGUTILS
  LIB_GRAPH
  LIB_MATH
  LIB_HLL
  )

if (CPPLIB_FOUND)
  set(CPPLIB_LIBRARIES
    ${LIB_STRUTILS}
    ${LIB_IOUTILS}
    ${LIB_LZ4IO}
    ${LIB_GZIPIO}
    ${LIB_ASSERT}
    ${LIB_OSUTILS}
    ${LIB_RNGUTILS}
    ${LIB_ARGSPARSER}
    ${LIB_GRAPH}
    ${LIB_MATH}
    ${LIB_HLL}
    )
  set(CPPLIB_INCLUDE_DIRS ${CPPLIB_ROOT_DIR})
  set(CPPLIB_DEFINITIONS)
endif()

# Tell cmake GUIs to ignore the "local" variables.
mark_as_advanced(
  CPPLIB_ROOT_DIR
  LIB_STRUTILS
  LIB_IOUTILS
  LIB_LZ4IO
  LIB_GZIPIO
  LIB_ASSERT
  LIB_OSUTILS
  LIB_ARGSPARSER
  LIB_GRAPH
  LIB_RNGUTILS
  LIB_MATH
  LIB_HLL
  )
