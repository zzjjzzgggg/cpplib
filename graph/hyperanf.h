/**
 * Copyright (C) by J.Z. (04/01/2018 16:01)
 * Distributed under terms of the MIT license.
 */

#ifndef __HYPERANF_H__
#define __HYPERANF_H__

#include "comm.h"
#include "dgraph.h"
#include "cncom.h"
#include "../adv/hll.h"

namespace graph {

/**
 * Approximate Neighbourhood Function based on following paper: HyperANF:
 * Approximating the Neighbourhood Function of Very Large Graphs on a Budget.
 *
 * Each register has length 1 byte
 */
class HyperANF {
protected:
    int p_,                  // precision,
        m_,                  // = 2^p: number of registers in a HLL counter,
        units_per_counter_;  // = m / 8: # of uint64 integers per HLL counter.
    std::vector<uint64_t> bits_;  // HLL counters are stored in a bit-vector
    std::unordered_map<int, int> cc_bitpos_, nd_cc_;

    rngutils::default_rng rng;

protected:
    /**
     * Merge HLL counter at pos_j to HLL counter at pos_i.
     * B(C_i) := max(B(C_i), B(C_j))
     */
    inline void mergeCounter(const int pos_i, const int pos_j) {
        for (int k = 0; k < units_per_counter_; k++) {
            uint64_t &x = bits_[pos_i + k], y = bits_[pos_j + k];
            hll::merge(x, y);
        }
    }

    /**
     * Generate HLL counter for a CC at pos. If a CC contains num nodes, then
     * need to add num random numbers.
     */
    inline void genHLLCounter(const int pos, const int num = 1) {
        uint8_t* reg_array = (uint8_t*)(bits_.data() + pos);
        for (int n = 0; n < num; n++) {
            uint64_t x = rng.randint<uint64_t>();
            int reg_idx = (x >> (64 - p_)) & ((1 << p_) - 1);
            // uint8_t reg_rho = hll::clz8(x << p_ | 1 << (p_ - 1)) + 1;
            uint8_t reg_rho = hll::clz8(x << p_) + 1;
            if (reg_rho > reg_array[reg_idx]) reg_array[reg_idx] = reg_rho;
        }
    }

public:
    HyperANF(const int p = 12) : p_(p) {
        m_ = 1 << p_;
        // ensure that a counter (m registers) occupies at least one uint64_t
        assert(m_ % 8 == 0);
        // a counter occupies m/8 64-bit integers
        units_per_counter_ = m_ / 8;
    }

    /**
     * Constructing CC DAG then initialing bits.
     */
    template <class Graph>
    void initBitsCC(const Graph& graph);

    double estimate(const int nd) const {
        uint8_t* regs = (uint8_t*)(bits_.data() + cc_bitpos_.at(nd_cc_.at(nd)));
        return hll::count(regs, m_);
    }

}; /* HyperANF */

template <class Graph>
void HyperANF::initBitsCC(const Graph& graph) {
    // do a DFS on the input graph
    SCCVisitor<Graph> dfs(graph);
    dfs.performDFS();

    // state the number of nodes in each CC, and build node-CC mapping
    std::unordered_map<int, int> cc_size;
    for (auto& pr : dfs.getCNEdges()) {
        cc_size[pr.first]++;
        nd_cc_[pr.second] = pr.first;  // node -> CC
    }

    // CCs in topological order
    auto cc_vec = dfs.getCCSorted();

    // generate bits for each CC
    bits_.resize(cc_vec.size() * units_per_counter_);
    std::fill(bits_.begin(), bits_.end(), 0);
    int pos = 0;
    for (int cc : cc_vec) {
        genHLLCounter(pos, cc_size.at(cc));
        cc_bitpos_[cc] = pos;
        pos += units_per_counter_;
    }

    // SCC DAG: represents CCs relationship
    dir::DGraph dag;
    for (auto& pr : dfs.getCCEdges()) dag.addEdgeFast(pr.first, pr.second);

    // update CC bits in topological order
    for (auto it = cc_vec.rbegin(); it != cc_vec.rend(); it++) {
        int ccj = *it;
        const auto& nd = dag[ccj];
        for (auto ni = nd.beginInNbr(); ni != nd.endInNbr(); ni++) {
            int cci = nd.getNbrID(ni);
            mergeCounter(cc_bitpos_[cci], cc_bitpos_[ccj]);
        }
    }
}

} /* namespace graph */
#endif /* __HYPERANF_H__ */
