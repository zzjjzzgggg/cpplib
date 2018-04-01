#include <cstdio>

#include "osutils.h"
#include "graph.h"

using namespace graph;

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
    SCCVisitor visitor(g);
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

void test_net() {
    DatNet<int, int, std::greater<std::pair<int, int>>> net;
    net.addEdge(1, 2, 1);
    net.addEdge(2, 3, 1);
    net.addEdge(2, 4, 2);
    net.addEdge(4, 5, 1);
    printf("%d, %d\n", net.getNodes(), net.getEdges());

    for (auto&& ei = net.beginEI(); ei != net.endEI(); ei++) {
        printf("%d, %d, %d\n", ei.getSrcID(), ei.getDstID(), ei.getDat());
    }

    for (auto&& ni = net.beginNI(); ni != net.endNI(); ni++) {
        printf("%d, %d\n", ni->first, ni->second.getOutDeg());
    }
}

void test_bgraph() {
    BGraph graph;
    graph.addEdge(1, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 2);
    graph.addEdge(2, 3);
    graph.addEdge(2, 4);
    graph.defrag();
    printf("L: %d, R: %d, E: %d\n\n", graph.getNodesL(), graph.getNodesR(),
           graph.getEdges());

    for (auto ni = graph.beginNIL(); ni != graph.endNIL(); ni++) {
        auto& nd = ni->second;
        printf("%d: %d\n", nd.getID(), nd.getDeg());
    }

    printf("\n");

    for (auto ni = graph.beginNIR(); ni != graph.endNIR(); ni++) {
        auto& nd = ni->second;
        printf("%d: %d\n", nd.getID(), nd.getDeg());
    }

    printf("\n");

    for (auto ei = graph.beginEI(); ei != graph.endEI(); ei++) {
        printf("%d - %d\n", ei.getSrcID(), ei.getDstID());
    }
}

int main(int argc, char* argv[]) {
    // osutils::Timer tm;

    test_scc();
    // test_bgraph();

    return 0;
}
