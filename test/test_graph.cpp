#include <cstdio>

#include "osutils.h"
#include "graph.h"

using namespace graph;

int main(int argc, char* argv[]) {
    osutils::Timer tm;
    DGraph G = loadEdgeList<DGraph>("test_graph.dat", GraphType::MULTI);
    printf("n: %d, e: %d, %s\n", G.getNodes(), G.getEdges(),
           tm.getStr().c_str());

    for (auto&& ei = G.beginEI(); ei != G.endEI(); ei++) {
        printf("%d\t%d\n", ei.getSrcID(), ei.getDstID());
    }

    printf("%d\n", countNodeDirTriads(3, G));

    return 0;
}
