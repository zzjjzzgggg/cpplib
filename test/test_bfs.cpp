/**
 * Copyright (C) by J.Z. (04/02/2018 14:54)
 * Distributed under terms of the MIT license.
 */
#include <cstdio>
#include "../os/osutils.h"
#include "../graph/graph.h"

using namespace graph;

void test_bfs() {
    std::string gfn = "/dat/workspace/datasets/cit-HepTh_wcc_digraph_mapped.gz";
    dir::DGraph graph = loadEdgeList<dir::DGraph>(gfn);
    DirBFS bfs(graph);

    int num = 2;

    std::unordered_set<int> nodes;
    for (int i = 0; i < 10; i++) {
        int nd = graph.sampleNode();

        bfs.doBFS(nd);
        int sz = bfs.getBFSTreeSize();
        printf("BFS from a random node %d reaches %d nodes in total.\n", nd,
               sz);

        nodes.clear();
        while (nodes.size() < (size_t)num) nodes.insert(graph.sampleNode());
        bfs.doBFS(nodes.begin(), nodes.end());
        sz = bfs.getBFSTreeSize();
        printf("BFS from random %d nodes reaches %d nodes in total.\n", num,
               sz);
    }
}

int main(int argc, char* argv[]) {
    osutils::Timer tm;

    test_bfs();

    printf("cost time %s\n", tm.getStr().c_str());
    return 0;
}
