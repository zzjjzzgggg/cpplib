#include <cstdio>

#include "osutils.h"
#include "graph.h"

using namespace graph;
using namespace graph::dir;

/**
 * test SCC on directed graph
 */
void test_scc() {
    DGraph g;
    g.addEdge(0, 1);
    g.addEdge(0, 5);
    g.addEdge(2, 0);
    g.addEdge(2, 3);
    g.addEdge(3, 2);
    g.addEdge(3, 5);
    g.addEdge(4, 2);
    g.addEdge(4, 3);
    g.addEdge(5, 4);
    g.addEdge(6, 0);
    g.addEdge(6, 4);
    g.addEdge(6, 9);
    g.addEdge(7, 6);
    g.addEdge(7, 8);
    g.addEdge(8, 7);
    g.addEdge(8, 9);
    g.addEdge(9, 10);
    g.addEdge(9, 11);
    g.addEdge(10, 12);
    g.addEdge(11, 4);
    g.addEdge(11, 12);
    g.addEdge(12, 9);

    SCCVisitor<DGraph> visitor(g);
    visitor.performDFS();
    int cc = -1;
    for (auto& pr : visitor.getCNEdges()) {
        int c = pr.first, v = pr.second;
        if (c != cc) {
            cc = c;
            printf("\nSCC[%d]: ", c);
        }
        printf(" %d", v);
    }
    printf("\n");

    for (auto& pr : visitor.getCCEdges()) {
        printf("%d --> %d\n", pr.first, pr.second);
    }

    for (int cc : visitor.getCCSorted()) printf("%d ", cc);
    printf("\n");
}

int main(int argc, char* argv[]) {
    test_scc();
    return 0;
}
