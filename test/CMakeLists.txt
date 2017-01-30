add_executable(test_fmt test_fmt.cpp)

add_executable(test_assert test_assert.cpp)
target_link_libraries(test_assert ppk_assert)

add_executable(test_stringutils test_stringutils.cpp)
target_link_libraries(test_stringutils stringutils)

add_executable(test_osutils test_osutils.cpp)
target_link_libraries(test_osutils osutils)

add_executable(test_ioutils test_ioutils.cpp)
target_link_libraries(test_ioutils ioutils)

add_executable(test_argsparser test_argsparser.cpp)
target_link_libraries(test_argsparser argsparser)
