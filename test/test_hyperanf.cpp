/**
 * Copyright (C) by J.Z. (04/02/2018 14:54)
 * Distributed under terms of the MIT license.
 */
#include <cstdio>
#include "../os/osutils.h"
#include "../graph/graph.h"

using namespace fmt::literals;
using namespace graph;
using namespace graph::dir;

void test_anf_single() {
    // std::string gfn =
    // "/dat/workspace/datasets/cit-HepTh_wcc_digraph_mapped.gz";
    std::string gfn = "/dat/workspace/graph_pds/test_graph.txt";
    DGraph graph = loadEdgeList<DGraph>(gfn);

    osutils::Timer tm;
    DirBFS<DGraph> bfs(graph);

    tm.tick();
    HyperANF anf(12);
    anf.initBitsCC(graph);
    printf("ANF: %.4fs\n", tm.seconds());

    printf("nd\ttruth\ttime\test\ttime\terror(%%)\n");

    // auto fw = ioutils::getIOOut("hepth_anf.dat");
    for (int i = 0; i < 100; i++) {
        int nd = graph.sampleNode();

        tm.tick();
        bfs.doBFS(nd);
        int truth = bfs.getBFSTreeSize();
        double t1 = tm.seconds();

        tm.tick();
        double est = anf.estimate(nd);
        double t2 = tm.seconds();

        double err = std::abs(est - truth) / truth * 100;

        printf("%d\t%d\t%.2e\t%.2f\t%.2e\t%.2f\n", nd, truth, t1, est, t2, err);
        // fw->save("{:d}\t{:d}\t{:.3e}\t{:.3f}\t{:.3e}\t{:.2f}\n"_format(
        //     nd, truth, t1, est, t2, err));
    }
}

void test_anf_set() {
    std::string gfn = "/dat/workspace/datasets/cit-HepTh_wcc_digraph_mapped.gz";
    // std::string gfn = "/home/jzzhao/workspace/streaming/test_graph.txt";
    DGraph graph = loadEdgeList<DGraph>(gfn);

    osutils::Timer tm;
    DirBFS<DGraph> bfs(graph);

    tm.tick();
    HyperANF anf(12);
    anf.initBitsCC(graph);
    printf("ANF: %.4fs\n", tm.seconds());

    printf("nd\ttruth\ttm\test\ttm\terr(%%)\n");

    std::unordered_set<int> nodes;

    for (size_t sz : {5, 10, 20}) {
        for (int i = 0; i < 10; i++) {
            nodes.clear();
            while (nodes.size() < sz) nodes.insert(graph.sampleNode());

            tm.tick();
            bfs.doBFS(nodes.begin(), nodes.end());
            int truth = bfs.getBFSTreeSize();
            double t1 = tm.seconds();

            tm.tick();
            double est = anf.estimate(nodes.begin(), nodes.end());
            double t2 = tm.seconds();

            double err = std::abs(est - truth) / truth * 100;

            printf("%lu\t%d\t%.2e\t%.2f\t%.2e\t%.2f\n", sz, truth, t1, est, t2,
                   err);
        }
    }
}

int main(int argc, char* argv[]) {
    // osutils::Timer tm;

    test_anf_single();
    // test_anf_set();

    return 0;
}
