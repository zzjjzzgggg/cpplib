/**
 * Copyright (C) by J.Z. (06/30/2017 13:46)
 * Distributed under terms of the MIT license.
 */

#include "bgraph.h"

namespace graph::bi {

void BGraph::save(const std::string& filename) const {
    auto po = ioutils::getIOOut(filename);
    po->save((int)nodes_L_.size());                // total number of nodes
    for (auto& pr : nodes_L_) pr.second.save(po);  // each node
    po->save((int)nodes_R_.size());                // total number of nodes
    for (auto& pr : nodes_R_) pr.second.save(po);  // each node
}

void BGraph::load(const std::string& filename) {
    auto pi = ioutils::getIOIn(filename);
    int num_nodes;
    pi->load(num_nodes);  // total number of nodes
    nodes_L_.reserve(num_nodes);
    for (int n = 0; n < num_nodes; n++) {
        Node node;
        node.load(pi);
        nodes_L_[node.getID()] = std::move(node);
    }

    pi->load(num_nodes);  // total number of nodes
    nodes_R_.reserve(num_nodes);
    for (int n = 0; n < num_nodes; n++) {
        Node node;
        node.load(pi);
        nodes_R_[node.getID()] = std::move(node);
    }
}

}  // end of namespace graph
