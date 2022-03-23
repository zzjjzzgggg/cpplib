/**
 * Copyright (C) by J.Z. (03/24/2018 18:19)
 * Distributed under terms of the MIT license.
 */

#ifndef __BFS_H__
#define __BFS_H__

#include "comm.h"

/**
 * BFS over directed graphs
 */
template <class Graph>
class DirBFS {
public:
    const Graph& graph_;
    std::unordered_map<int, int> nd_to_hop_;

public:
    DirBFS(const Graph& graph) : graph_(graph) {}

    // BFS along out-edges.
    void doBFS(const int start_nd, const int mx_hop = INT_MAX);

    template <class InputIt>
    void doBFS(InputIt first, InputIt last, const int mx_hop = INT_MAX);

    // BFS along in-edges.
    void doRevBFS(const int start_nd, const int mx_hop = INT_MAX);

    template <class InputIt>
    void doRevBFS(InputIt first, InputIt last, const int mx_hop = INT_MAX);

    int getBFSTreeSize() const { return nd_to_hop_.size(); }

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
        for (auto&& ni = nd.beginOutNbr(); ni != nd.endOutNbr(); ++ni) {
            int v = *ni;
            if (nd_to_hop_.find(v) == nd_to_hop_.end()) {
                nd_to_hop_[v] = hop + 1;
                queue.push(v);
            }
        }
    }
}

template <class Graph>
template <class InputIt>
void DirBFS<Graph>::doBFS(InputIt first, InputIt last, const int mx_hop) {
    nd_to_hop_.clear();
    std::queue<int> queue;
    for (; first != last; ++first) {
        nd_to_hop_[*first] = 0;
        queue.push(*first);
    }
    while (!queue.empty()) {
        int u = queue.front(), hop = nd_to_hop_[u];
        queue.pop();
        if (hop >= mx_hop) break;
        const auto& nd = graph_[u];
        for (auto&& ni = nd.beginOutNbr(); ni != nd.endOutNbr(); ++ni) {
            int v = *ni;
            if (nd_to_hop_.find(v) == nd_to_hop_.end()) {
                nd_to_hop_[v] = hop + 1;
                queue.push(v);
            }
        }
    }
}

template <class Graph>
void DirBFS<Graph>::doRevBFS(const int start_nd, const int mx_hop) {
    nd_to_hop_.clear();
    nd_to_hop_[start_nd] = 0;
    std::queue<int> queue;
    queue.push(start_nd);
    while (!queue.empty()) {
        int u = queue.front(), hop = nd_to_hop_[u];
        queue.pop();
        if (hop >= mx_hop) break;
        auto& nd = graph_[u];
        for (auto&& ni = nd.beginInNbr(); ni != nd.endInNbr(); ++ni) {
            int v = *ni;
            if (nd_to_hop_.find(v) == nd_to_hop_.end()) {
                nd_to_hop_[v] = hop + 1;
                queue.push(v);
            }
        }
    }
}

template <class Graph>
template <class InputIt>
void DirBFS<Graph>::doRevBFS(InputIt first, InputIt last, const int mx_hop) {
    nd_to_hop_.clear();
    std::queue<int> queue;
    for (; first != last; ++first) {
        nd_to_hop_[*first] = 0;
        queue.push(*first);
    }
    while (!queue.empty()) {
        int u = queue.front(), hop = nd_to_hop_[u];
        queue.pop();
        if (hop >= mx_hop) break;
        const auto& nd = graph_[u];
        for (auto&& ni = nd.beginInNbr(); ni != nd.endInNbr(); ++ni) {
            int v = *ni;
            if (nd_to_hop_.find(v) == nd_to_hop_.end()) {
                nd_to_hop_[v] = hop + 1;
                queue.push(v);
            }
        }
    }
}

#endif /* __BFS_H__ */
