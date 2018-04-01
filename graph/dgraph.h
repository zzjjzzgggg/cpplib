/**
 * Copyright (C) by J.Z. (06/30/2017 13:47)
 * Distributed under terms of the MIT license.
 */

#ifndef __DGRAPH_H__
#define __DGRAPH_H__

#include "node_interface.h"
#include "edge_iter_interface.h"

namespace graph {

/**
 * directed graph
 */
class DGraph {
public:
    typedef typename std::vector<int>::const_iterator NbrIter;

    class Node : public INode<NbrIter> {
    private:
        std::vector<int> in_nbrs_, out_nbrs_;

    public:
        Node() : INode(-1) {}
        Node(const int id) : INode(id) {}

        // disable copy constructor/assignment
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;

        // move constructor/assignment
        Node(Node&& other)
            : INode(other.id_), in_nbrs_(std::move(other.in_nbrs_)),
              out_nbrs_(std::move(other.out_nbrs_)) {}

        Node& operator=(Node&& other) {
            id_ = other.id_;
            in_nbrs_ = std::move(other.in_nbrs_);
            out_nbrs_ = std::move(other.out_nbrs_);
            return *this;
        }

        void save(std::unique_ptr<ioutils::IOOut>& po) const override;
        void load(std::unique_ptr<ioutils::IOIn>& pi) override;

        int getDeg() const override { return getInDeg() + getOutDeg(); }
        int getInDeg() const override { return in_nbrs_.size(); }
        int getOutDeg() const override { return out_nbrs_.size(); }

        int getInNbr(const int d) const override { return in_nbrs_[d]; }
        int getOutNbr(const int d) const override { return out_nbrs_[d]; }
        int getNbr(const int d) const override {
            return d < getOutDeg() ? getOutNbr(d) : getInNbr(d - getOutDeg());
        }

        bool isInNbr(const int nbr) const override {
            return std::binary_search(in_nbrs_.begin(), in_nbrs_.end(), nbr);
        }

        bool isOutNbr(const int nbr) const override {
            return std::binary_search(out_nbrs_.begin(), out_nbrs_.end(), nbr);
        }

        bool isNbr(const int nbr) const override { return isOutNbr(nbr); }

        NbrIter beginNbr() const override { return out_nbrs_.begin(); }
        NbrIter endNbr() const override { return out_nbrs_.end(); }

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

        void addInNbr(const int nbr) { in_nbrs_.push_back(nbr); }
        void addOutNbr(const int nbr) { out_nbrs_.push_back(nbr); }

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
     * Iterate over out-neighbors
     *
     *   cur_edge_
     *      |
     *      v
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

private:
    GraphType gtype_;
    mutable rngutils::default_rng rng_;
    std::unordered_map<int, Node> nodes_;  // maps a node id to its node object

public:
    DGraph(const GraphType gtype = GraphType::SIMPLE) : gtype_(gtype) {}
    virtual ~DGraph() {}

    // disable copy constructor/assignment
    DGraph(const DGraph&) = delete;
    DGraph& operator=(const DGraph&) = delete;

    // move constructor/assignment
    DGraph(DGraph&& other)
        : gtype_(other.gtype_), nodes_(std::move(other.nodes_)) {}

    DGraph& operator=(DGraph&& other) {
        gtype_ = other.gtype_;
        nodes_ = std::move(other.nodes_);
        return *this;
    }

    void save(const std::string& filename) const;
    void load(const std::string& filename);

    bool isNode(const int id) const { return nodes_.find(id) != nodes_.end(); }
    bool isEdge(const int src, const int dst) const {
        if (!isNode(src) || !isNode(dst)) return false;
        return nodes_.at(src).isOutNbr(dst);
    }

    const int getNodes() const { return nodes_.size(); }
    const int getEdges() const {
        int edges = 0;
        for (const auto& p : nodes_) edges += p.second.getOutDeg();
        return edges;
    }

    Node& getNode(const int id) { return nodes_[id]; }
    const Node& getNode(const int id) const { return nodes_.at(id); }
    Node& operator[](int id) { return nodes_[id]; };
    const Node& operator[](int id) const { return nodes_.at(id); };

    /**
     * Make sure the graph has nodes before calling this method
     */
    int sampleNode() const {
        auto iter = rng_.choose(nodes_.begin(), nodes_.end());
        return iter->first;
    }

    int sampleInNbr(int id) const { return getNode(id).sampleInNbr(rng_); }
    int sampleOutNbr(int id) const { return getNode(id).sampleOutNbr(rng_); }

    void addNode(int id) {
        if (!isNode(id)) nodes_[id] = Node{id};
    }

    /**
     * Add edge (src, dst) to the graph. For efficiency consideration, this
     * function does not check whether or not the edge has existed. Callers are
     * responsible to avoid duplidate edges.
     */
    void addEdge(const int src, const int dst) {
        addNode(src);
        addNode(dst);
        nodes_[src].addOutNbr(dst);
        nodes_[dst].addInNbr(src);
    }

    /**
     * Optimize the graph data structure for the purpose of fast access.
     * Including: sort neighbors of each node increasingly.
     */
    void defrag() {
        for (auto& p : nodes_) p.second.shrinkAndSort();
        if (gtype_ == GraphType::SIMPLE)
            for (auto& p : nodes_) p.second.uniq();
    }

    // iterators
    NodeIter beginNI() const { return nodes_.begin(); }
    NodeIter endNI() const { return nodes_.end(); }

    /**
     * Find the first node that out degree is nonzero
     */
    EdgeIter beginEI() const {
        auto ni = nodes_.begin();
        while (ni != nodes_.end() && ni->second.getOutDeg() == 0) ni++;
        return EdgeIter(ni, nodes_.end());
    }
    EdgeIter endEI() const { return EdgeIter(nodes_.end(), nodes_.end()); }
};

}  // end namespace graph

#endif /* __DGRAPH_H__ */
