#include <cstdio>

#include "osutils.h"
#include "graph.h"

using namespace graph;

int main(int argc, char* argv[]) {
    osutils::Timer tm;

    DatNet<int, int, std::greater<std::pair<int, int>>> net;
    net.addEdge(1, 2, 1);
    net.addEdge(2, 3, 1);
    net.addEdge(2, 4, 2);
    net.addEdge(4, 5, 1);
    printf("%d, %d\n", net.getNodes(), net.getEdges());

    for (auto&& ei=net.beginEI(); ei!=net.endEI(); ei++) {
        printf("%d, %d, %d\n", ei.getSrcID(), ei.getDstID(), ei.getDat());
    }

    return 0;
}
