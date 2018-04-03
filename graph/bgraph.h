/**
 * Copyright (C) by J.Z. (04/01/2018 20:40)
 * Distributed under terms of the MIT license.
 */

#ifndef __BGRAPH_H__
#define __BGRAPH_H__

#include "node_interface.h"
#include "edge_iter_interface.h"

namespace graph {

/**
 * Bipartite Graph
 */
class BGraph {
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

        int getNbrID(const NbrIter& it) const override { return *it; }

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
    mutable rngutils::default_rng rng_;  // we don't care rng_
    std::unordered_map<int, Node> nodes_L_,
        nodes_R_;  // maps a node id to its node object

public:
    BGraph(const GraphType gtype = GraphType::SIMPLE) : gtype_(gtype) {}
    virtual ~BGraph() {}

    // disable copy constructor/assignment
    BGraph(const BGraph&) = delete;
    BGraph& operator=(const BGraph&) = delete;

    // move constructor/assignment
    BGraph(BGraph&& other)
        : gtype_(other.gtype_), nodes_L_(std::move(other.nodes_L_)),
          nodes_R_(std::move(other.nodes_R_)) {}

    BGraph& operator=(BGraph&& other) {
        gtype_ = other.gtype_;
        nodes_L_ = std::move(other.nodes_L_);
        nodes_R_ = std::move(other.nodes_R_);
        return *this;
    }

    void save(const std::string& filename) const;
    void load(const std::string& filename);

    bool isNode(const int id) const { return isNodeL(id) || isNodeR(id); }
    bool isNodeL(const int id) const {
        return nodes_L_.find(id) != nodes_L_.end();
    }
    bool isNodeR(const int id) const {
        return nodes_R_.find(id) != nodes_R_.end();
    }
    bool isEdge(const int src, const int dst) const {
        if (!isNodeL(src) || !isNodeR(dst)) return false;
        return nodes_L_.at(src).isNbr(dst);
    }

    const int getNodes() const { return nodes_L_.size() + nodes_R_.size(); }
    const int getNodesL() const { return nodes_L_.size(); }
    const int getNodesR() const { return nodes_R_.size(); }

    const int getEdges() const {
        int edges = 0;
        for (const auto& p : nodes_L_) edges += p.second.getDeg();
        return edges;
    }

    Node& getNodeL(const int id) { return nodes_L_[id]; }
    Node& getNodeR(const int id) { return nodes_R_[id]; }
    const Node& getNodeL(const int id) const { return nodes_L_.at(id); }
    const Node& getNodeR(const int id) const { return nodes_R_.at(id); }

    int sampleNodeL() const {
        auto iter = rng_.choose(nodes_L_.begin(), nodes_L_.end());
        return iter->first;
    }

    int sampleNodeR() const {
        auto iter = rng_.choose(nodes_R_.begin(), nodes_R_.end());
        return iter->first;
    }

    int sampleNbrL(int id) const { return getNodeL(id).sampleNbr(rng_); }
    int sampleNbrR(int id) const { return getNodeR(id).sampleNbr(rng_); }

    void addNodeL(int id) {
        if (!isNodeL(id)) nodes_L_[id] = Node{id};
    }
    void addNodeR(int id) {
        if (!isNodeR(id)) nodes_R_[id] = Node{id};
    }

    /**
     * Add edge (src, dst) to the graph. For the purpose of fast adding, this
     * function does not check whether or not the edge has existed.
     *
     * NOTE: Callers are responsible to avoid duplidate edges.
     */
    void addEdge(const int src, const int dst) {
        addNodeL(src);
        nodes_L_[src].addNbr(dst);
        addNodeR(dst);
        nodes_R_[dst].addNbr(src);
    }

    /**
     * Optimize the graph data structure for the purpose of fast access,
     * including sorting and uniqing neighbors of each node increasingly.
     */
    void defrag() {
        for (auto& p : nodes_L_) p.second.shrinkAndSort();
        for (auto& p : nodes_R_) p.second.shrinkAndSort();
        if (gtype_ == GraphType::SIMPLE) {
            for (auto& p : nodes_L_) p.second.uniq();
            for (auto& p : nodes_R_) p.second.uniq();
        }
    }

    // iterators

    NodeIter beginNIL() const { return nodes_L_.begin(); }
    NodeIter endNIL() const { return nodes_L_.end(); }
    NodeIter beginNIR() const { return nodes_R_.begin(); }
    NodeIter endNIR() const { return nodes_R_.end(); }

    /**
     * find the first node that degree is nonzero, and first neighbor <= node
     */
    EdgeIter beginEI() const {
        auto ni = nodes_L_.begin();
        while (ni != nodes_L_.end() && ni->second.getDeg() == 0) ni++;
        return EdgeIter(ni, nodes_L_.end());
    }
    EdgeIter endEI() const { return EdgeIter(nodes_L_.end(), nodes_L_.end()); }

}; /* BGraph */

} /* namespace graph */

#endif /* __BGRAPH_H__ */
