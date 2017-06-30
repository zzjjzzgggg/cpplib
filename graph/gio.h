/*
 * Copyright (C) 2017 by J. Zhao
 * Distributed under terms of the MIT license.
 */

#ifndef __GIO_H__
#define __GIO_H__

#include "../io/ioutils.h"

namespace graph {

template <typename Graph>
Graph loadEdgeList(const std::string& edges_fnm, const bool is_multi = false) {
    Graph G(is_multi);
    ioutils::TSVParser ss{edges_fnm};
    while (ss.next()) {
        int src = ss.get<int>(0), dst = ss.get<int>(1);
        G.addNode(src);
        G.addNode(dst);
        G.addEdge(src, dst);
    }
    G.optimize();
    return G;
}

template <typename Graph>
Graph loadBinEdgeList(const std::string& edges_fnm,
                      const bool is_multi = false) {
    Graph G(is_multi);
    int src, dst;
    auto pin = ioutils::getIOIn(edges_fnm);
    while (!pin->eof()) {
        pin->load(src);
        pin->load(dst);
        G.addNode(src);
        G.addNode(dst);
        G.addEdge(src, dst);
    }
    G.optimize();
    return G;
}

template <typename Graph>
Graph loadBinary(const std::string& filename) {
    Graph G;
    G.load(filename);
    return G;
}

}  // end namespace graph

#endif /* __GIO_H__ */
