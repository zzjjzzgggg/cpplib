#include <cstdio>

#include "osutils.h"
#include "graph.h"

using namespace graph;

int main(int argc, char* argv[]) {
    osutils::Timer tm;
    DGraph G = loadEdgeList<DGraph>("test_graph.dat", true);
    printf("n: %d, e: %d, %s\n", G.getNodes(), G.getEdges(),
           tm.getStr().c_str());

    for (auto&& ei = G.beginEI(); ei != G.endEI(); ei++) {
        printf("%d\t%d\n", ei.getSrcID(), ei.getDstID());
    }

    printf("%d\n", countNodeDirTriads(G, 3));

    auto triads_hist = statDirTriads(G);
    for (auto& pr : triads_hist) printf("%d  %d\n", pr.first, pr.second);

    return 0;
}
