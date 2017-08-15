add_executable(test_fmt test_fmt.cpp)

add_executable(test_assert test_assert.cpp)
target_link_libraries(test_assert ppk_assert)

add_executable(test_strutils test_strutils.cpp)
target_link_libraries(test_strutils strutils)

add_executable(test_osutils test_osutils.cpp)
target_link_libraries(test_osutils osutils)

add_executable(test_ioutils test_ioutils.cpp)
target_link_libraries(test_ioutils ioutils)

add_executable(test_argsparser test_argsparser.cpp)
target_link_libraries(test_argsparser argsparser)

add_executable(test_rngutils test_rngutils.cpp)
target_link_libraries(test_rngutils rngutils)

add_executable(test_graph test_graph.cpp)
target_link_libraries(test_graph graph)

add_executable(test_mathutils test_mathutils.cpp)
target_link_libraries(test_mathutils mathutils)
