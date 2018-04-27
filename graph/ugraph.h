/**
 * Copyright (C) by J.Z. (06/30/2017 13:45)
 * Distributed under terms of the MIT license.
 */

#ifndef __UGRAPH_H__
#define __UGRAPH_H__

#include "graph_interface.h"

namespace graph::undir {

typedef typename std::vector<int>::const_iterator NbrIter;

/**
 * Undirected graph node class
 */
class Node : public INode<NbrIter> {
private:
    std::vector<int> nbrs_;  // neighbors

public:
    Node() : INode<NbrIter>(-1) {}
    Node(const int id) : INode<NbrIter>(id) {}

    // copy constructor
    Node(const Node& o) : INode<NbrIter>(o), nbrs_(o.nbrs_) {}

    // copy assignment
    Node& operator=(const Node& o) {
        INode<NbrIter>::operator=(o);
        nbrs_ = o.nbrs_;
        return *this;
    }

    // move constructor
    Node(Node&& o) : INode<NbrIter>(std::move(o)), nbrs_(std::move(o.nbrs_)) {}

    // move assignment
    Node& operator=(Node&& o) {
        INode<NbrIter>::operator=(std::move(o));
        nbrs_ = std::move(o.nbrs_);
        return *this;
    }

    void save(std::unique_ptr<ioutils::IOOut>& po) const override;
    void load(std::unique_ptr<ioutils::IOIn>& pi) override;

    int getDeg() const override { return nbrs_.size(); }

    int getNbrID(const NbrIter& it) const override { return *it; }

    std::vector<int>& getNbrs() { return nbrs_; }
    const std::vector<int>& getNbrs() const { return nbrs_; }

    NbrIter beginNbr() const override { return nbrs_.begin(); }
    NbrIter endNbr() const override { return nbrs_.end(); }

    /**
     * Make sure the node has neighbors before calling this method
     */
    int sampleNbr(rngutils::random_generator<>& rng) const {
        auto iter = rng.choose(nbrs_.begin(), nbrs_.end());
        return *iter;
    }

    bool isNbr(const int nbr) const override {
        return std::binary_search(nbrs_.begin(), nbrs_.end(), nbr);
    }

    void addNbr(const int nbr) override;
    void addNbrFast(const int nbr) override { nbrs_.push_back(nbr); }

    void clear() override { nbrs_.clear(); }

    void shrinkAndSort() {
        nbrs_.shrink_to_fit();
        std::sort(nbrs_.begin(), nbrs_.end());
    }

    void uniq() {
        auto last = std::unique(nbrs_.begin(), nbrs_.end());
        nbrs_.erase(last, nbrs_.end());
    }
};
typedef std::unordered_map<int, Node>::const_iterator NodeIter;

/**
 * Undirected graph EdgeIter class.
 *
 *   cur_edge_
 *      |
 *      V
 * v1: {0 1 ... d1}  <- cur_nd_
 * v2: {0 2 ... d2}
 * ...               <- end_nd_
 */
class EdgeIter : public IEdgeIter<EdgeIter, NodeIter, NbrIter> {
public:
    EdgeIter() {}
    EdgeIter(const NodeIter& start_nd_iter, const NodeIter& end_nd_iter)
        : IEdgeIter(start_nd_iter, end_nd_iter) {}

    // copy assignment
    EdgeIter& operator=(const EdgeIter& ei) {
        return IEdgeIter<EdgeIter, NodeIter, NbrIter>::operator=(ei);
    }

    int getSrcID() const override { return cur_nd_->second.getID(); }
    int getDstID() const override { return *cur_edge_; }
};

/**
 * undirected graph.
 */
class UGraph : public IGraph<UGraph, Node, NodeIter, EdgeIter, NbrIter> {
public:
    typedef typename std::vector<int>::const_iterator NbrIter;

public:
    UGraph(const GraphType gtype = GraphType::SIMPLE)
        : IGraph<UGraph, Node, NodeIter, EdgeIter, NbrIter>(gtype) {}

    virtual ~UGraph() {}

    // disable copy constructor/assignment
    UGraph(const UGraph&) = delete;
    UGraph& operator=(const UGraph&) = delete;

    // move constructor
    UGraph(UGraph&& o)
        : IGraph<UGraph, Node, NodeIter, EdgeIter, NbrIter>(std::move(o)) {}

    // move assignment
    UGraph& operator=(UGraph&& o) {
        return static_cast<UGraph&>(
            IGraph<UGraph, Node, NodeIter, EdgeIter, NbrIter>::operator=(
                std::move(o)));
    }

    void save(const std::string& filename) const override;
    void load(const std::string& filename) override;

    bool isEdge(const int src, const int dst) const override {
        if (!isNode(src) || !isNode(dst)) return false;
        return nodes_.at(src).isNbr(dst);
    }

    const int getEdges() const override {
        int edges = 0;
        for (const auto& p : nodes_) edges += p.second.getDeg();
        return edges / 2;
    }

    int sampleNbr(int id) const { return getNode(id).sampleNbr(rng_); }

    /**
     * Add edge (src, dst) to the graph. For the purpose of fast adding, this
     * function does not check whether or not the edge has existed.
     *
     * NOTE: Callers are responsible to avoid duplidate edges.
     */
    void addEdgeFast(const int src, const int dst) override;

    /**
     * The default addEdge method will sort neighbors.
     */
    void addEdge(const int src, const int dst) override;

    EdgeIter beginEI() const override;
};

}  // end namespace graph

#endif /* __UGRAPH_H__ */
