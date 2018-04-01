/**
 * Copyright (C) by J.Z. (06/30/2017 13:45)
 * Distributed under terms of the MIT license.
 */

#ifndef __UGRAPH_H__
#define __UGRAPH_H__

#include "node_interface.h"
#include "edge_iter_interface.h"

namespace graph {

/**
 * undirected graph.
 */
class UGraph {
public:
    typedef typename std::vector<int>::const_iterator NbrIter;

    class Node : public INode<NbrIter> {
    private:
        std::vector<int> nbrs_;  // neighbors

    public:
        Node() : INode(-1) {}
        Node(const int id) : INode(id) {}

        // disable copy constructor/assignment
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;

        // move constructor/assignment
        Node(Node&& other) : INode(other.id_), nbrs_(std::move(other.nbrs_)) {}
        Node& operator=(Node&& other) {
            id_ = other.id_;
            nbrs_ = std::move(other.nbrs_);
            return *this;
        }

        void save(std::unique_ptr<ioutils::IOOut>& po) const override;
        void load(std::unique_ptr<ioutils::IOIn>& pi) override;

        int getDeg() const override { return nbrs_.size(); }
        int getNbr(const int d) const override { return nbrs_[d]; }

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

        void addNbr(const int nbr) { nbrs_.push_back(nbr); }

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

private:
    GraphType gtype_;
    mutable rngutils::default_rng rng_;    // we don't care rng_
    std::unordered_map<int, Node> nodes_;  // maps a node id to its node object

public:
    UGraph(const GraphType gtype = GraphType::SIMPLE) : gtype_(gtype) {}
    virtual ~UGraph() {}

    // disable copy constructor/assignment
    UGraph(const UGraph&) = delete;
    UGraph& operator=(const UGraph&) = delete;

    // move constructor/assignment
    UGraph(UGraph&& other)
        : gtype_(other.gtype_), nodes_(std::move(other.nodes_)) {}

    UGraph& operator=(UGraph&& other) {
        gtype_ = other.gtype_;
        nodes_ = std::move(other.nodes_);
        return *this;
    }

    void save(const std::string& filename) const;
    void load(const std::string& filename);

    bool isNode(const int id) const { return nodes_.find(id) != nodes_.end(); }
    bool isEdge(const int src, const int dst) const {
        if (!isNode(src) || !isNode(dst)) return false;
        return nodes_.at(src).isNbr(dst);
    }

    const int getNodes() const { return nodes_.size(); }
    const int getEdges() const {
        int edges = 0;
        for (const auto& p : nodes_) edges += p.second.getDeg();
        return edges / 2;
    }

    Node& getNode(const int id) { return nodes_[id]; }
    const Node& getNode(const int id) const { return nodes_.at(id); }
    Node& operator[](int id) { return nodes_[id]; };
    const Node& operator[](int id) const { return nodes_.at(id); };

    int sampleNode() const {
        auto iter = rng_.choose(nodes_.begin(), nodes_.end());
        return iter->first;
    }

    int sampleNbr(int id) const { return getNode(id).sampleNbr(rng_); }

    void addNode(int id) {
        if (!isNode(id)) nodes_[id] = Node{id};
    }

    /**
     * Add edge (src, dst) to the graph. For the purpose of fast adding, this
     * function does not check whether or not the edge has existed.
     *
     * NOTE: Callers are responsible to avoid duplidate edges.
     */
    void addEdge(const int src, const int dst) {
        addNode(src);
        nodes_[src].addNbr(dst);
        if (dst != src) {
            addNode(dst);
            nodes_[dst].addNbr(src);
        }
    }

    /**
     * Optimize the graph data structure for the purpose of fast access,
     * including sorting and uniqing neighbors of each node increasingly.
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
     * find the first node that degree is nonzero, and first neighbor <= node
     */
    EdgeIter beginEI() const {
        auto ni = nodes_.begin();
        while (ni != nodes_.end() && ni->second.getDeg() == 0) ni++;
        return EdgeIter(ni, nodes_.end());
    }
    EdgeIter endEI() const { return EdgeIter(nodes_.end(), nodes_.end()); }
};

}  // end namespace graph

#endif /* __UGRAPH_H__ */
