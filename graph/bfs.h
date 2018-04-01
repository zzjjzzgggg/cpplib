/**
 * Copyright (C) by J.Z. (03/24/2018 18:19)
 * Distributed under terms of the MIT license.
 */

#ifndef __BFS_H__
#define __BFS_H__

#include "comm.h"

/**
 * BFS over directed graphs, i.e., only search from out-neighbors of each node.
 */
template <class Graph>
class DirBFS {
public:
    const Graph graph_;
    std::unordered_map<int, int> nd_to_hop_;

public:
    DirBFS(const Graph& graph) : graph_(graph) {}
    virtual ~DirBFS() {}

    void doBFS(const int start_nd, const int mx_hop = INT_MAX);

}; /* DirBFS */

template <class Graph>
void DirBFS<Graph>::doBFS(const int start_nd, const int mx_hop) {
    nd_to_hop_.clear();
    nd_to_hop_[start_nd] = 0;
    std::queue<int> queue;
    queue.push(start_nd);
    while (!queue.empty()) {
        int u = queue.front(), hop = nd_to_hop_[u];
        queue.pop();
        if (hop >= mx_hop) break;
        auto& nd = graph_[u];
        for (auto&& nbr_iter = nd.beginOutNbr(); nbr_iter != nd.endOutNbr();
             nbr_iter++) {
            int v = nbr_iter->first;
            if (nd_to_hop_.find(v)==nd_to_hop_.end()) {
                nd_to_hop_[v] = hop + 1;
                queue.push(v);
            }
        }
    }
}

#endif /* __BFS_H__ */
