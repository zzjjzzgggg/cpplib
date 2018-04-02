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

namespace graph {

/**
 * HyperANF: Approximating the Neighbourhood Function of Very Large Graphs on a
 * Budget
 *
 * A register has length 1 byte
 */
template <class Graph>
class HyperANF {
public:
    typedef unsigned char uchar;
    typedef unsigned long long uint64;
    typedef std::vector<uint64> Bits;

    static constexpr uchar CLZ_TABLE_4BIT[16] = {4, 3, 2, 2, 1, 1, 1, 1,
                                                 0, 0, 0, 0, 0, 0, 0, 0};
    static constexpr uint64 L8 = 0x0101010101010101;
    static constexpr uint64 H8 = 0x8080808080808080;

private:
    const Graph& graph_;

    int p_, m_;  // m=2^p where p is precision
    double alpha_;
    Bits bits_;  // HLL counters are stored in a bit-vector
    std::unordered_map<int, int> cc_bitpos_, nd_cc_;
    int units_per_counter_;  // number of uint64 units per HLL counter = m_ /
                             // 8
    rngutils::default_rng rng;

private:
    double alpha() const {
        switch (m_) {
            case 16:
                return 0.673;
            case 32:
                return 0.697;
            case 64:
                return 0.709;
            default:
                return 0.7213 / (1 + 1.079 / m_);
        }
    }

    double beta(const double ez) const {
        double zl = std::log(ez + 1);
        return -0.37331876643753059 * ez + -1.41704077448122989 * zl +
               0.40729184796612533 * std::pow(zl, 2) +
               1.56152033906584164 * std::pow(zl, 3) +
               -0.99242233534286128 * std::pow(zl, 4) +
               0.26064681399483092 * std::pow(zl, 5) +
               -0.03053811369682807 * std::pow(zl, 6) +
               0.00155770210179105 * std::pow(zl, 7);
    }

    /**
     * Count leading zeros within 8 bits
     */
    uchar clz8(uint64 x) const {
        if ((x & 0xFF00000000000000) == 0) return 8;
        uchar n;
        if ((x & 0xF000000000000000) == 0) {
            n = 4;
            x <<= 4;
        } else
            n = 0;
        return n + CLZ_TABLE_4BIT[x >> 60];
    }

    /**
     * Merge HLL counter at pos_j to HLL counter at pos_i.
     *
     * B(C_i) := max(B(C_i), B(C_j))
     */
    inline void mergeCounter(const int pos_i, const int pos_j) {
        for (int k = 0; k < units_per_counter_; k++) {
            uint64 &x = bits_[pos_i + k], y = bits_[pos_j + k];
            uint64 z = ((((x | H8) - (y & ~H8)) | (x ^ y)) ^ (x | ~y)) & H8;
            uint64 m = ((((z >> 7) | H8) - L8) | H8) ^ z;
            x = (x & m) | (y & ~m);
        }
    }

    /**
     * Constructing CC DAG then initialing bits.
     */
    void initBitsCC();

    /**
     * Given counter posiiton, use LC and HLL to estimate cardinality. If
     * HLL estimate < 10000, then use LC; otherwise use HLL.
     */
    double count(const uchar* reg) const;

public:
    HyperANF(const Graph& graph, const int p = 16) : graph_(graph), p_(p) {
        m_ = 1 << p_;
        // ensure that a HLL counter (m registers) occupies at least one uint64
        assert(m_ % 8 == 0);
        // a HLL counter occupies m_/8 64-bit integers
        units_per_counter_ = m_ / 8;
        alpha_ = alpha();
    }

    void init() { initBitsCC(); }

    double estimate(const int nd) const {
        return count((uchar*)(bits_.data() + cc_bitpos_.at(nd_cc_.at(nd))));
    }

}; /* HyperANF */

template <class Graph>
double HyperANF<Graph>::count(const uchar* reg) const {
    double sum = 0, ez = 0;
    for (int i = 0; i < m_; i++) {
        sum += 1.0 / (1 << reg[i]);
        if (reg[i] == 0) ez++;
    }
    sum += beta(ez);
    double est_HLL = alpha_ * m_ * (m_ - ez) / sum + 0.5,
           est_LC = m_ * std::log(m_ / ez);
    return est_HLL < 10000 ? est_LC : est_HLL;
}

template <class Graph>
void HyperANF<Graph>::initBitsCC() {
    // do a DFS on the input graph
    SCCVisitor sccvis(graph_);
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
        uchar* reg_array = (uchar*)(bits_.data() + pos);
        // if CC contains n nodes, then need to add n random numbers
        for (int n = 0; n < cng.getNodeL(cc).getDeg(); n++) {
            uint64 x = rng.randint<uint64>();
            int reg_idx = (x >> (64 - p_)) & ((1 << p_) - 1);
            uchar reg_rho = clz8(x << p_ | 1 << (p_ - 1)) + 1;
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
        for (int d = 0; d < ccj_nd.getInDeg(); d++) {
            int cci = ccj_nd.getInNbr(d);
            mergeCounter(cc_bitpos_[cci], cc_bitpos_[ccj]);
        }
    }
}

} /* namespace graph */
#endif /* __HYPERANF_H__ */
