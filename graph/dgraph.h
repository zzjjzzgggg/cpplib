/**
 * Copyright (C) by J.Z. (06/30/2017 13:47)
 * Distributed under terms of the MIT license.
 */

#ifndef __DGRAPH_H__
#define __DGRAPH_H__

#include "graph_interface.h"

namespace graph::dir {

typedef typename std::vector<int>::const_iterator NbrIter;

/**
 * Directed graph node class
 */
class Node : public INode<NbrIter> {
private:
    std::vector<int> in_nbrs_, out_nbrs_;

public:
    Node() : INode(-1) {}
    Node(const int id) : INode(id) {}

    // copy constructor
    Node(const Node& o)
        : INode<NbrIter>(o), in_nbrs_(o.in_nbrs_), out_nbrs_(o.out_nbrs_) {}

    // copy assignment
    Node& operator=(const Node& o) {
        INode<NbrIter>::operator=(o);
        in_nbrs_ = o.in_nbrs_;
        out_nbrs_ = o.out_nbrs_;
        return *this;
    }

    // move constructor/assignment
    Node(Node&& o)
        : INode<NbrIter>(std::move(o)), in_nbrs_(std::move(o.in_nbrs_)),
          out_nbrs_(std::move(o.out_nbrs_)) {}

    Node& operator=(Node&& o) {
        INode<NbrIter>::operator=(std::move(o));
        in_nbrs_ = std::move(o.in_nbrs_);
        out_nbrs_ = std::move(o.out_nbrs_);
        return *this;
    }

    void save(std::unique_ptr<ioutils::IOOut>& po) const override;
    void load(std::unique_ptr<ioutils::IOIn>& pi) override;

    int getDeg() const override { return in_nbrs_.size() + out_nbrs_.size(); }
    int getInDeg() const override { return in_nbrs_.size(); }
    int getOutDeg() const override { return out_nbrs_.size(); }

    int getNbrID(const NbrIter& it) const override { return *it; }

    bool isInNbr(const int v) const override {
        return std::binary_search(in_nbrs_.begin(), in_nbrs_.end(), v);
    }
    bool isOutNbr(const int v) const override {
        return std::binary_search(out_nbrs_.begin(), out_nbrs_.end(), v);
    }
    bool isNbr(const int v) const override {
        return isOutNbr(v) || isOutNbr(v);
    }

    void addInNbr(const int v) override;
    void addOutNbr(const int v) override;
    void addNbr(const int v) override { addOutNbr(v); }

    void addInNbrFast(const int v) override { in_nbrs_.push_back(v); }
    void addOutNbrFast(const int v) override { out_nbrs_.push_back(v); }
    void addNbrFast(const int v) override { addOutNbrFast(v); }

    void clear() override;

    NbrIter beginNbr() const override {
        return in_nbrs_.empty() ? out_nbrs_.begin() : in_nbrs_.begin();
    }
    NbrIter endNbr() const override { return out_nbrs_.end(); }
    void nextNbr(NbrIter& ni) const override {
        if (++ni == in_nbrs_.end()) ni = out_nbrs_.begin();
    };

    NbrIter beginInNbr() const override { return in_nbrs_.begin(); }
    NbrIter endInNbr() const override { return in_nbrs_.end(); }

    NbrIter beginOutNbr() const override { return out_nbrs_.begin(); }
    NbrIter endOutNbr() const override { return out_nbrs_.end(); }

    /**
     * Make sure the node has out-neighbors before calling this method
     */
    int sampleOutNbr(rngutils::random_generator<>& rng) const {
        auto iter = rng.choose(out_nbrs_.begin(), out_nbrs_.end());
        return *iter;
    }

    /**
     * Make sure the node has in-neighbors before calling this method
     */
    int sampleInNbr(rngutils::random_generator<>& rng) const {
        auto iter = rng.choose(in_nbrs_.begin(), in_nbrs_.end());
        return *iter;
    }

    void shrinkAndSort() {
        in_nbrs_.shrink_to_fit();
        out_nbrs_.shrink_to_fit();
        std::sort(in_nbrs_.begin(), in_nbrs_.end());
        std::sort(out_nbrs_.begin(), out_nbrs_.end());
    }

    void uniq() {
        auto last_i = std::unique(in_nbrs_.begin(), in_nbrs_.end());
        in_nbrs_.erase(last_i, in_nbrs_.end());
        auto last_o = std::unique(out_nbrs_.begin(), out_nbrs_.end());
        out_nbrs_.erase(last_o, out_nbrs_.end());
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
 * directed graph
 */
class DGraph : public IGraph<DGraph, Node, NodeIter, EdgeIter, NbrIter> {
public:
    typedef typename std::vector<int>::const_iterator NbrIter;

public:
    DGraph(const GraphType gtype = GraphType::SIMPLE)
        : IGraph<DGraph, Node, NodeIter, EdgeIter, NbrIter>(gtype) {}
    virtual ~DGraph() {}

    // copy constructor
    DGraph(const DGraph& o)
        : IGraph<DGraph, Node, NodeIter, EdgeIter, NbrIter>(o) {}

    // copy assignment
    DGraph& operator=(const DGraph& o) {
        return static_cast<DGraph&>(
            IGraph<DGraph, Node, NodeIter, EdgeIter, NbrIter>::operator=(o));
    }

    // move constructor
    DGraph(DGraph&& o)
        : IGraph<DGraph, Node, NodeIter, EdgeIter, NbrIter>(std::move(o)) {}

    // move assignment
    DGraph& operator=(DGraph&& o) {
        return static_cast<DGraph&>(
            IGraph<DGraph, Node, NodeIter, EdgeIter, NbrIter>::operator=(
                std::move(o)));
    }

    void save(const std::string& filename) const override;
    void load(const std::string& filename) override;

    bool isEdge(const int src, const int dst) const override {
        if (!isNode(src) || !isNode(dst)) return false;
        return nodes_.at(src).isOutNbr(dst);
    }

    const int getEdges() const override {
        int edges = 0;
        for (const auto& p : nodes_) edges += p.second.getOutDeg();
        return edges;
    }

    int sampleInNbr(int id) const { return getNode(id).sampleInNbr(rng_); }
    int sampleOutNbr(int id) const { return getNode(id).sampleOutNbr(rng_); }

    /**
     * Add edge (src, dst) to the graph. For efficiency consideration, this
     * function does not check whether or not the edge has existed. Callers are
     * responsible to avoid duplidate edges.
     */
    void addEdgeFast(const int src, const int dst) override;

    /**
     * Add edge and keep neighbors in order. If the edge already exists, then do
     * nothing.
     */
    void addEdge(const int src, const int dst) override;

    /**
     * Find the first node that out degree is nonzero
     */
    EdgeIter beginEI() const override;
};

}  // namespace graph::dir

#endif /* __DGRAPH_H__ */
