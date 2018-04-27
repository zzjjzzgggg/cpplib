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

add_executable(test_cc test_cc.cpp)
target_link_libraries(test_cc graph)

add_executable(test_mathutils test_mathutils.cpp)
target_link_libraries(test_mathutils mathutils)

add_executable(test_bfs test_bfs.cpp)
target_link_libraries(test_bfs graph)

add_executable(test_hyperanf test_hyperanf.cpp)
target_link_libraries(test_hyperanf graph)

add_executable(test_HLL test_HLL.cpp)
# target_link_libraries(test_HLL graph)

add_executable(test_LRU test_LRU.cpp)
