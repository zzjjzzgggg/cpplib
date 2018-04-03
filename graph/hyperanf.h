/**
 * Copyright (C) by J.Z. (04/01/2018 16:01)
 * Distributed under terms of the MIT license.
 */

#ifndef __HYPERANF_H__
#define __HYPERANF_H__

#include "comm.h"
#include "dgraph.h"
#include "bgraph.h"
#include "cncom.h"
#include "../adv/hll.h"

namespace graph {

/**
 * HyperANF: Approximating the Neighbourhood Function of Very Large Graphs on a
 * Budget
 *
 * Each register has length 1 byte
 */
template <class Graph>
class HyperANF {
private:
    const Graph& graph_;

    int p_, m_;                   // m = 2^p where p is precision
    std::vector<uint64_t> bits_;  // HLL counters are stored in a bit-vector
    std::unordered_map<int, int> cc_bitpos_, nd_cc_;

    int units_per_counter_;  // # of uint64 integers per HLL counter = m_ / 8
    rngutils::default_rng rng;

private:
    /**
     * Merge HLL counter at pos_j to HLL counter at pos_i.
     *
     * B(C_i) := max(B(C_i), B(C_j))
     */
    inline void mergeCounter(const int pos_i, const int pos_j) {
        for (int k = 0; k < units_per_counter_; k++) {
            uint64_t &x = bits_[pos_i + k], y = bits_[pos_j + k];
            hll::merge(x, y);
        }
    }

public:
    HyperANF(const Graph& graph, const int p = 16) : graph_(graph), p_(p) {
        m_ = 1 << p_;
        // ensure that a HLL counter (m registers) occupies at least one
        // uint64_t
        assert(m_ % 8 == 0);
        // a HLL counter occupies m_/8 64-bit integers
        units_per_counter_ = m_ / 8;
    }

    /**
     * Constructing CC DAG then initialing bits.
     */
    void initBitsCC();

    double estimate(const int nd) const {
        uint8_t* regs = (uint8_t*)(bits_.data() + cc_bitpos_.at(nd_cc_.at(nd)));
        return hll::count(regs, m_);
    }

}; /* HyperANF */

template <class Graph>
void HyperANF<Graph>::initBitsCC() {
    // do a DFS on the input graph
    SCCVisitor<Graph> sccvis(graph_);
    sccvis.performDFS();

    // build bipartite graph: represents component-node inclusion relationship,
    // and node-CC mapping.
    BGraph cng;
    for (auto& pr : sccvis.getCNEdges()) {
        cng.addEdge(pr.first, pr.second);  // (CC, node)
        nd_cc_[pr.second] = pr.first;      // node -> CC
    }
    cng.defrag();

    // CCs in topological order
    auto cc_vec = sccvis.getCCSorted();

    // generate bits for each CC
    bits_.resize(cc_vec.size() * units_per_counter_);
    std::fill(bits_.begin(), bits_.end(), 0);
    int pos = 0;
    for (int cc : cc_vec) {
        uint8_t* reg_array = (uint8_t*)(bits_.data() + pos);
        // if CC contains n nodes, then need to add n random numbers
        for (int n = 0; n < cng.getNodeL(cc).getDeg(); n++) {
            uint64_t x = rng.randint<uint64_t>();
            int reg_idx = (x >> (64 - p_)) & ((1 << p_) - 1);
            uint8_t reg_rho = hll::clz8(x << p_ | 1 << (p_ - 1)) + 1;
            if (reg_rho > reg_array[reg_idx]) reg_array[reg_idx] = reg_rho;
        }
        cc_bitpos_[cc] = pos;
        pos += units_per_counter_;
    }

    // SCC DAG: represents CCs relationship
    DGraph dag;
    for (auto& pr : sccvis.getCCEdges()) dag.addEdge(pr.first, pr.second);
    dag.defrag();

    // update CC bits in topological order
    for (auto it = cc_vec.rbegin(); it != cc_vec.rend(); it++) {
        int ccj = *it;
        const auto& ccj_nd = dag[ccj];
        for (auto ni = ccj_nd.beginInNbr(); ni != ccj_nd.endInNbr(); ni++) {
            int cci = ccj_nd.getNbrID(ni);
            mergeCounter(cc_bitpos_[cci], cc_bitpos_[ccj]);
        }
    }
}

} /* namespace graph */
#endif /* __HYPERANF_H__ */
