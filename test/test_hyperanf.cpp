/**
 * Copyright (C) by J.Z. (04/02/2018 14:54)
 * Distributed under terms of the MIT license.
 */
#include <cstdio>
#include "../os/osutils.h"
#include "../graph/graph.h"

using namespace graph;
using namespace graph::dir;

void test_anf() {
    // std::string gfn = "/dat/workspace/datasets/cit-HepTh_wcc_digraph_mapped.gz";
    std::string gfn = "/home/jzzhao/workspace/streaming/test_graph.txt";
    DGraph graph = loadEdgeList<DGraph>(gfn);

    osutils::Timer tm;
    DirBFS bfs(graph);

    tm.tick();
    HyperANF anf(12);
    anf.initBitsCC(graph);
    printf("ANF: %.4fs\n", tm.seconds());

    printf("nd\ttruth (tm)\t\test (tm)\t\t err\n");

    for (int i = 0; i < 10; i++) {
        int nd = graph.sampleNode();

        tm.tick();
        bfs.doBFS(nd);
        int truth = bfs.getBFSTreeSize();
        double t1 = tm.seconds();

        tm.tick();
        double est = anf.estimate(nd);
        double t2 = tm.seconds();

        printf("%d\t%d (tm %.3f)\t\t%.1f (tm %.3f)\t\t%.2f%%\n",
               nd, truth, t1, est, t2, std::abs(est - truth) / truth * 100);
    }
}

int main(int argc, char* argv[]) {
    osutils::Timer tm;

    test_anf();

    return 0;
}
