/**
 * Copyright (C) by J.Z. (03/24/2018 18:19)
 * Distributed under terms of the MIT license.
 */

#ifndef __BFS_H__
#define __BFS_H__

#include "comm.h"

namespace graph {
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

    /**
     * Perform an incremental BFS for seed node with respect to a node set.
     * Return a node-to-hop unordered map.
     */
    template <class InputIt>
    std::unordered_map<int, int> doIncBFS(InputIt first, InputIt last,
                                          const int seed,
                                          const int mx_hop = INT_MAX);

    // BFS along in-edges.
    void doRevBFS(const int start_nd, const int mx_hop = INT_MAX);

    template <class InputIt>
    void doRevBFS(InputIt first, InputIt last, const int mx_hop = INT_MAX);

    int getBFSTreeSize() const { return nd_to_hop_.size(); }

}; /* DirBFS */

template <class Graph>
void DirBFS<Graph>::doBFS(const int start_nd, const int mx_hop) {
    if (!graph_.isNode(start_nd)) return;
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
        if (graph_.isNode(*first)) {
            nd_to_hop_[*first] = 0;
            queue.push(*first);
        }
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
template <class InputIt>
std::unordered_map<int, int> DirBFS<Graph>::doIncBFS(InputIt first, InputIt last,
                                                     const int seed,
                                                     const int mx_hop) {
    nd_to_hop_.clear();
    // First, perform a BFS from the node set.
    doBFS(first, last, mx_hop);
    std::unordered_map<int, int> nd_to_hop;

    // If the seed node is already in the BFS tree, return empty.
    if (!graph_.isNode(seed) || nd_to_hop_.find(seed) != nd_to_hop_.end())
        return nd_to_hop;

    // Then, perform a BFS from the seed node, and only expand the new BFS tree
    // from unvisted nodes.
    nd_to_hop[seed] = 0;
    std::queue<int> queue;
    queue.push(seed);

    while (!queue.empty()) {
        int u = queue.front(), hop = nd_to_hop[u];
        queue.pop();
        if (hop >= mx_hop) break;
        const auto& nd = graph_[u];
        for (auto&& ni = nd.beginOutNbr(); ni != nd.endOutNbr(); ++ni) {
            int v = *ni;
            if (nd_to_hop_.find(v) == nd_to_hop_.end() &&
                nd_to_hop.find(v) == nd_to_hop.end()) {
                nd_to_hop[v] = hop + 1;
                queue.push(v);
            }
        }
    }
    return nd_to_hop;
}

template <class Graph>
void DirBFS<Graph>::doRevBFS(const int start_nd, const int mx_hop) {
    if (!graph_.isNode(start_nd)) return;
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
        if (graph_.isNode(*first)) {
            nd_to_hop_[*first] = 0;
            queue.push(*first);
        }
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

} /* namespace graph */
#endif /* __BFS_H__ */
