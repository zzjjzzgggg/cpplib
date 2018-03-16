add_library(lz4io SHARED lz4io.cpp)
target_link_libraries(lz4io lz4)

add_library(gzipio SHARED gzipio.cpp)
target_link_libraries(gzipio ppk_assert strutils)

add_library(ioutils SHARED ioutils.cpp)
target_link_libraries(ioutils lz4io gzipio osutils)

add_library(argsparser SHARED argsparser.cpp)
target_link_libraries(argsparser lz4io strutils)
