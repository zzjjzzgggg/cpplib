#include <cstdio>

#include "graph.h"

int main(int argc, char* argv[]) {
    graph::DGraph G;
    G.addNode(1);
    G.addNode(2);
    G.addNode(3);
    G.addEdge(1, 1);
    G.addEdge(1, 2);
    G.addEdge(1, 3);
    printf("n: %d, e: %d\n", G.getNodes(), G.getEdges());
    printf("d: %d\n", G.getNode(1).getInDeg());
    printf("%d\n", G.sampleNode());

    auto& node = G.getNode(1);
    printf("%d\n", node.sampleOutNbr());
    return 0;
}
