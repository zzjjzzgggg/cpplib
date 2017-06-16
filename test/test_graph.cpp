#include <cstdio>

#include "osutils.h"
#include "graph.h"

int main(int argc, char* argv[]) {
    osutils::Timer tm;
    graph::DGraph G = graph::loadEdgeList<graph::DGraph>(
        "~/workspace/datasets/cit-HepTh_digraph.gz");
    printf("n: %d, e: %d, %s\n", G.getNodes(), G.getEdges(),
           tm.getStr().c_str());
    // G.save("test.lz");

    tm.tick();
    graph::DGraph H = graph::loadBinary<graph::DGraph>("test.lz");
    printf("n: %d, e: %d, %s\n", H.getNodes(), H.getEdges(),
           tm.getStr().c_str());

    return 0;
}
