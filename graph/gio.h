/*
 * Copyright (C) 2017 by J. Zhao
 * Distributed under terms of the MIT license.
 */

#ifndef __GIO_H__
#define __GIO_H__

#include "comm.h"

namespace graph {

template <class Graph>
Graph loadEdgeList(const std::string& edges_fnm,
                   const GraphType gtype = GraphType::SIMPLE) {
    Graph G(gtype);
    ioutils::TSVParser ss(edges_fnm);
    while (ss.next()) G.addEdge(ss.get<int>(0), ss.get<int>(1));
    G.defrag();
    return G;
}

template <class Graph>
Graph loadBinEdgeList(const std::string& edges_fnm,
                      const GraphType gtype = GraphType::SIMPLE) {
    int src, dst;
    Graph G(gtype);
    auto pin = ioutils::getIOIn(edges_fnm);
    while (!pin->eof()) {
        pin->load(src);
        pin->load(dst);
        G.addEdge(src, dst);
    }
    G.defrag();
    return G;
}

template <class Graph>
Graph loadBinary(const std::string& filename) {
    Graph G;
    G.load(filename);
    return G;
}

template <class Graph>
void saveEdgelist(const Graph& G, const std::string& filename) {
    auto po = ioutils::getIOOut(filename);
    for (auto&& ei = G.beginEI(); ei != G.endEI(); ei++)
        po->save(fmt::format("{}\t{}\n", ei.getSrcID(), ei.getDstID()));
    printf("edgelist saved to %s\n", filename.c_str());
}

}  // end namespace graph

#endif /* __GIO_H__ */
