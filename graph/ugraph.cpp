/**
 * Copyright (C) by J.Z. (06/30/2017 13:46)
 * Distributed under terms of the MIT license.
 */

#include "ugraph.h"

namespace graph::undir {

void Node::save(std::unique_ptr<ioutils::IOOut>& po) const {
    po->save(id_);                        // id
    po->save((int)nbrs_.size());          // deg
    for (int nbr : nbrs_) po->save(nbr);  // neighbors
}

void Node::load(std::unique_ptr<ioutils::IOIn>& pi) {
    int deg, nbr;
    pi->load(id_);       // id
    pi->load(deg);       // deg
    nbrs_.reserve(deg);  // neighbors
    for (int d = 0; d < deg; d++) {
        pi->load(nbr);
        nbrs_.push_back(nbr);
    }
}

void Node::addNbr(const int nbr) {
    nbrs_.push_back(nbr);
    int i = nbrs_.size() - 1, tmp;
    while (i > 0 && nbrs_[i] < nbrs_[i - 1]) {
        tmp = nbrs_[i];
        nbrs_[i] = nbrs_[i - 1];
        nbrs_[i - 1] = tmp;
        i--;
    }
}

void UGraph::save(const std::string& filename) const {
    auto po = ioutils::getIOOut(filename);
    po->save((int)nodes_.size());                // total number of nodes
    for (auto& pr : nodes_) pr.second.save(po);  // each node
}

void UGraph::load(const std::string& filename) {
    auto pi = ioutils::getIOIn(filename);
    int num_nodes;
    pi->load(num_nodes);  // total number of nodes
    nodes_.reserve(num_nodes);
    for (int n = 0; n < num_nodes; n++) {
        Node node;
        node.load(pi);
        nodes_[node.getID()] = std::move(node);
    }
}

void UGraph::addEdge(const int src, const int dst) {
    addNode(src);
    nodes_[src].addNbr(dst);
    addNode(dst);
    nodes_[dst].addNbr(src);
}

void UGraph::addEdgeFast(const int src, const int dst) {
    addNode(src);
    nodes_[src].addNbrFast(dst);
    addNode(dst);
    nodes_[dst].addNbrFast(src);
}

EdgeIter UGraph::beginEI() const {
    auto ni = nodes_.begin();
    while (ni != nodes_.end() && ni->second.getDeg() == 0) ni++;
    return EdgeIter(ni, nodes_.end());
}

}  // end of namespace graph::undir
