/**
 * Copyright (C) by J.Z. (06/30/2017 20:06)
 * Distributed under terms of the MIT license.
 */

#ifndef __TRIAD_H__
#define __TRIAD_H__

#include <vector>
#include <unordered_map>
#include <algorithm>

namespace graph {
/**
 * Count the number of directed triads the node belonging to
 */
template <class Graph>
int countNodeDirTriads(const int& node, const Graph& G) {
    if (G[node].getDeg() < 2) return 0;
    int triads = 0;
    // get unique neighbors
    std::unordered_map<int, int> nbr_cnt;
    const auto& nd_obj = G[node];
    for (auto ni = nd_obj.beginNbr(); ni != nd_obj.endNbr();
         nd_obj.nextNbr(ni)) {
        int nbr = nd_obj.getNbrID(ni);
        if (nbr != node) nbr_cnt[nbr]++;
    }
    std::vector<int> nbr_vec;
    nbr_vec.reserve(nbr_cnt.size());
    for (auto& it : nbr_cnt) nbr_vec.push_back(it.first);
    // count connected neighbors
    for (auto src_it = nbr_vec.begin(); src_it != nbr_vec.end(); src_it++) {
        auto& src_obj = G[*src_it];
        std::unordered_map<int, int> src_nbr_cnt;
        for (auto ni = src_obj.beginNbr(); ni != src_obj.endNbr();
             src_obj.nextNbr(ni))
            src_nbr_cnt[src_obj.getNbrID(ni)]++;
        for (auto dst_it = src_it + 1; dst_it != nbr_vec.end(); dst_it++) {
            if (src_nbr_cnt.find(*dst_it) != src_nbr_cnt.end())
                triads +=
                    src_nbr_cnt[*dst_it] * nbr_cnt[*src_it] * nbr_cnt[*dst_it];
        }
    }
    return triads;
}

} /* end of namespace graph */

#endif /* __TRIAD_H__ */
