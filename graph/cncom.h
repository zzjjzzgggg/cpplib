/**
 * Copyright (C) by J.Z. (03/31/2018 16:50)
 * Distributed under terms of the MIT license.
 */

#ifndef __CNCOM_H__
#define __CNCOM_H__

#include "comm.h"

namespace graph {

/**
 * Strongly connected componetn DFS visitor. Using Tarjan's SCC Algorithm. Refer
 * to Knuth's book "The Stanford Graph Base", pp.512-519.
 */
template <class Graph>
class SCCVisitor {
public:
    class Element {
    public:
        int id, parent, rank, min, edge, deg;

    public:
        Element(int v = -1, int r = -1, int d = 0)
            : id(v), parent(-1), rank(r), min(v), edge(0), deg(d) {}
    }; /* Node */

private:
    const Graph& graph_;
    int rank_;
    std::unordered_map<int, Element> record_;
    std::stack<int> stack_;

public:
    std::vector<std::pair<int, int>> nd_cc_vec_;  // node-component pairs

public:
    SCCVisitor(const Graph& graph) : graph_(graph), rank_(0) {
        nd_cc_vec_.reserve(graph_.getNodes());
    }

    void performDFS() {
        for (auto it = graph_.beginNI(); it != graph_.endNI(); it++) {
            int v = it->first;
            if (record_.find(v) == record_.end()) {
                makeActive(v);
                while (v != -1) v = explore(v);
            }
        }
        // topology sort
        std::reverse(nd_cc_vec_.begin(), nd_cc_vec_.end());
    }

    /**
     * Make vertex v active.
     */
    void makeActive(const int v, const int parent = -1) {
        record_[v] = Element(v, ++rank_, graph_[v].getOutDeg());
        if (parent != -1) record_[v].parent = parent;
        stack_.push(v);
    }

    /**
     * Explore one step from current vertex v, possible move to another current
     * vertex and calling it v. Return the next vertex to be explored.
     */
    int explore(const int v) {
        Element& ve = record_[v];
        if (ve.edge != ve.deg) {  // v has untagged edge
            const int u = graph_[v].getOutNbr(ve.edge++);
            if (record_.find(u) != record_.end()) {  // seen u before
                int u_rank = record_[u].rank, v_min_rank = record_[ve.min].rank;
                if (u_rank < v_min_rank) ve.min = u;
                return v;
            } else {  // u is presently unseen
                makeActive(u, v);
                // u become the current vertex, i.e., the node to be explored
                return u;
            }
        } else {  // all edges from v are tagged, so v matures
            int u = ve.parent;
            if (ve.min == v) {  // subtree with root v forms a SCC
                finish(v);
            } else {  // the edge from u to v has just matured, make v.min
                      // visible from u
                int v_min_rank = record_[ve.min].rank, &u_min = record_[u].min,
                    u_min_rank = record_[u_min].rank;
                if (v_min_rank < u_min_rank) u_min = ve.min;
            }
            return u;
        }
    }

    /**
     * Remove v and all its successors on the active stack from the tree, and
     * mark them as a SCC.
     */
    void finish(const int v) {
        while (true) {
            int t = stack_.top();
            stack_.pop();
            nd_cc_vec_.emplace_back(t, v);
            record_[t].rank = INT_MAX;
            record_[t].parent = v;
            if (t == v) break;
        }
    }

    /**
     * Get connected component connections. Keep one representative connection
     * between two SCCs.
     */
    std::vector<std::pair<int, int>> getCCEdges() {
        int cur_cc = -1;
        std::vector<std::pair<int, int>> cc_edge_vec;
        std::unordered_set<int> discovered_ccs;
        for (auto& pr : nd_cc_vec_) {
            int v = pr.first;
            int cc_from = pr.second;
            if (cc_from != cur_cc) {
                cur_cc = cc_from;
                discovered_ccs.clear();
            }
            discovered_ccs.insert(cc_from);
            for (int d = 0; d < record_[v].deg; d++) {
                int cc_to = record_[graph_[v].getOutNbr(d)].parent;
                if (discovered_ccs.find(cc_to) == discovered_ccs.end()) {
                    discovered_ccs.insert(cc_to);
                    cc_edge_vec.emplace_back(cc_from, cc_to);
                }
            }
        }
        return cc_edge_vec;
    }

    /**
     * Return topologically sorted CCs
     */
    std::vector<int> getCCSorted() {
        std::vector<int> vec;
        int cc = -1;
        for (auto& pr: nd_cc_vec_) {
            if (pr.second != cc) {
                cc = pr.second;
                vec.push_back(cc);
            }
        }
        return vec;
    }
};

} /* namespace graph */
#endif /* __CNCOM_H__ */
