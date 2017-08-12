/**
 * Copyright (C) by J.Z. (06/30/2017 13:47)
 * Distributed under terms of the MIT license.
 */

#ifndef __DGRAPH_H__
#define __DGRAPH_H__

#include "comm.h"

namespace graph {

/**
 * directed graph
 */
class DGraph {
public:
    class Node {
    private:
        int id_;
        std::vector<int> in_nbrs_, out_nbrs_;

    public:
        Node() : id_(-1) {}
        Node(const int id) : id_(id) {}

        // disable copy constructor/assignment
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;

        // move constructor/assignment
        Node(Node&& other)
            : id_(other.id_), in_nbrs_(std::move(other.in_nbrs_)),
              out_nbrs_(std::move(other.out_nbrs_)) {}

        Node& operator=(Node&& other) {
            id_ = other.id_;
            in_nbrs_ = std::move(other.in_nbrs_);
            out_nbrs_ = std::move(other.out_nbrs_);
            return *this;
        }

        void save(std::unique_ptr<ioutils::IOOut>& po) const;
        void load(std::unique_ptr<ioutils::IOIn>& pi);

        int getID() const { return id_; }

        int getInDeg() const { return in_nbrs_.size(); }
        int getOutDeg() const { return out_nbrs_.size(); }
        int getDeg() const { return getInDeg() + getOutDeg(); }

        int getInNbr(const int d) const { return in_nbrs_[d]; }
        int getOutNbr(const int d) const { return out_nbrs_[d]; }
        int getNbr(const int d) const {
            return d < getOutDeg() ? getOutNbr(d) : getInNbr(d - getOutDeg());
        }

        bool isInNbr(const int nbr) const {
            return std::binary_search(in_nbrs_.begin(), in_nbrs_.end(), nbr);
        }

        std::vector<int>::iterator beginInNbr() { return in_nbrs_.begin(); }
        std::vector<int>::iterator endInNbr() { return in_nbrs_.end(); }

        std::vector<int>::iterator beginOutNbr() { return out_nbrs_.begin(); }
        std::vector<int>::iterator endOutNbr() { return out_nbrs_.end(); }

        /**
         * Make sure the node has out-neighbors before calling this method
         */
        int sampleOutNbr(randutils::random_generator<>& rng) const {
            auto iter = rng.choose(out_nbrs_.begin(), out_nbrs_.end());
            return *iter;
        }

        /**
         * Make sure the node has in-neighbors before calling this method
         */
        int sampleInNbr(randutils::random_generator<>& rng) const {
            auto iter = rng.choose(in_nbrs_.begin(), in_nbrs_.end());
            return *iter;
        }

        bool isOutNbr(const int nbr) const {
            return std::binary_search(out_nbrs_.begin(), out_nbrs_.end(), nbr);
        }

        void addInNbr(const int nbr) { in_nbrs_.push_back(nbr); }
        void addOutNbr(const int nbr) { out_nbrs_.push_back(nbr); }

        void sort() {
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
     *      V
     * v1: {0 1 ... d1}  <- cur_
     * v2: {0 2 ... d2}
     * ...               <- end_
     */
    class EdgeIter {
    private:
        NodeIter cur_, end_;
        int cur_edge_;

    public:
        EdgeIter() : cur_edge_(0) {}
        EdgeIter(const NodeIter& start_nd_iter, const NodeIter& end_nd_iter)
            : cur_(start_nd_iter), end_(end_nd_iter), cur_edge_(0) {}

        // copy assignment
        EdgeIter& operator=(const EdgeIter& edge_iter) {
            if (this != &edge_iter) {
                cur_ = edge_iter.cur_;
                end_ = edge_iter.end_;
                cur_edge_ = edge_iter.cur_edge_;
            }
            return *this;
        }

        // postfix increment: i++
        // move to next valid edge; if not exists, move to end
        EdgeIter& operator++(int) {
            cur_edge_++;
            while (cur_ != end_ && cur_edge_ >= cur_->second.getOutDeg()) {
                cur_edge_ = 0;
                cur_++;
            }
            return *this;
        }

        bool operator!=(const EdgeIter& ei) const {
            return cur_ != ei.cur_ ||
                   (cur_ == ei.cur_ && cur_edge_ < ei.cur_edge_);
        }

        bool operator==(const EdgeIter& ei) const {
            return cur_ == ei.cur_ && cur_edge_ == ei.cur_edge_;
        }

        int getSrcID() const { return cur_->second.getID(); }
        int getDstID() const { return cur_->second.getOutNbr(cur_edge_); }
    };

private:
    GraphType gtype_;
    mutable randutils::default_rng rng_;
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
     * function does not check whether or not the edge has existed. Callers
     * are
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
        for (auto& p : nodes_) p.second.sort();
        if (gtype_ == GraphType::SIMPLE)
            for (auto& p : nodes_) p.second.uniq();
    }

    // iterators
    NodeIter beginNI() const { return nodes_.begin(); }
    NodeIter endNI() const { return nodes_.end(); }
    /**
     * find the first node that out degree is nonzero
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
