/**
 * Copyright (C) by J.Z. (04/02/2018 14:54)
 * Distributed under terms of the MIT license.
 */
#include <cstdio>
#include "../os/osutils.h"
#include "../graph/graph.h"

using namespace graph;

void test_HLL() {
    std::string gfn = "/dat/workspace/datasets/cit-HepTh_wcc_digraph_mapped.gz";
    DGraph graph = loadEdgeList<DGraph>(gfn);

    DirBFS bfs(graph);

    HyperANF anf(graph);
    anf.init();

    osutils::Timer tm;
    for (int i = 0; i < 10; i++) {
        int nd = graph.sampleNode();

        tm.tick();
        bfs.doBFS(nd);
        int truth = bfs.getBFSTreeSize();
        double t1 = tm.seconds();

        tm.tick();
        double est = anf.estimate(nd);
        double t2 = tm.seconds();

        printf("nd: %d\t t:%d (tm %.3f)\t\t e:%.0f (tm %.3f)\t\t er:%.2f%%\n",
               nd, truth, t1, est, t2, std::abs(est - truth) / truth * 100);
    }
}

int main(int argc, char* argv[]) {
    osutils::Timer tm;

    test_HLL();

    return 0;
}
