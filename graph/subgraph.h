/**
 * Copyright (C) by J.Z. 2019-12-18 20:38
 * Distributed under terms of the MIT license.
 */

#ifndef __SUBGRAPH_H__
#define __SUBGRAPH_H__

#include "comm.h"

namespace graph {

/**
 * Get the reduced subgraph from a vector of nodes
 */
template <class Graph>
Graph getSubgraph(const Graph& G, const std::vector<int>& nodes) {
    Graph S;
    for (int v : nodes) S.addNode(v);
    for (int v : nodes) {
        auto&& nd_v = G[v];
        for (auto ni = nd_v.beginOutNbr(); ni != nd_v.endOutNbr(); ++ni) {
            int nbr = nd_v.getNbrID(ni);
            if (S.isNode(nbr)) S.addEdge(v, nbr);
        }
    }
    return S;
}
}  // namespace graph

#endif /* __SUBGRAPH_H__ */
