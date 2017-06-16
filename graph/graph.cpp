/*
 * Copyright (C) 2017 by J. Zhao
 * Distributed under terms of the MIT license.
 */

#include "graph.h"

namespace graph {

void UGraph::Node::save(std::unique_ptr<ioutils::IOOut>& po) const {
    po->save(id_);                        // id
    po->save((int)nbrs_.size());          // deg
    for (int nbr : nbrs_) po->save(nbr);  // neighbors
}

void UGraph::Node::load(std::unique_ptr<ioutils::IOIn>& pi) {
    int deg, nbr;
    pi->load(id_);       // id
    pi->load(deg);       // deg
    nbrs_.reserve(deg);  // neighbors
    for (int d = 0; d < deg; d++) {
        pi->load(nbr);
        nbrs_.push_back(nbr);
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

void DGraph::Node::save(std::unique_ptr<ioutils::IOOut>& po) const {
    po->save(id_);                            // id
    po->save((int)in_nbrs_.size());           // in-deg
    for (int nbr : in_nbrs_) po->save(nbr);   // in-neighbors
    po->save((int)out_nbrs_.size());          // out-deg
    for (int nbr : out_nbrs_) po->save(nbr);  // out-neighbors
}

void DGraph::Node::load(std::unique_ptr<ioutils::IOIn>& pi) {
    int deg, nbr;
    pi->load(id_);          // id
    pi->load(deg);          // in-deg
    in_nbrs_.reserve(deg);  // in-neighbors
    for (int d = 0; d < deg; d++) {
        pi->load(nbr);
        in_nbrs_.push_back(nbr);
    }
    pi->load(deg);           // out-deg
    out_nbrs_.reserve(deg);  // out-neighbors
    for (int d = 0; d < deg; d++) {
        pi->load(nbr);
        out_nbrs_.push_back(nbr);
    }
}

void DGraph::save(const std::string& filename) const {
    auto po = ioutils::getIOOut(filename);
    po->save((int)nodes_.size());                // total number of nodes
    for (auto& pr : nodes_) pr.second.save(po);  // each node
}

void DGraph::load(const std::string& filename) {
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

}  // end of namespace graph
