/*
 * Copyright (C) 2017 by J. Zhao
 * Distributed under terms of the MIT license.
 */

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <vector>
#include <unordered_map>
#include <algorithm>

#include "../io/ioutils.h"
#include "../adv/randutils.h"

namespace graph {

/**
 * undirected graph.
 */
class UGraph {
public:
    class Node {
    private:
        int id_;                 // node
        std::vector<int> nbrs_;  // neighbors

    public:
        Node() : id_(-1) {}
        Node(const int id) : id_(id) {}

        // disable copy constructor/assignment
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;

        // move constructor/assignment
        Node(Node&& other) : id_(other.id_), nbrs_(std::move(other.nbrs_)) {}

        Node& operator=(Node&& other) {
            id_ = other.id_;
            nbrs_ = std::move(other.nbrs_);
            return *this;
        }

        void save(std::unique_ptr<ioutils::IOOut>& po) const;
        void load(std::unique_ptr<ioutils::IOIn>& pi);

        int getID() const { return id_; }
        int getDeg() const { return nbrs_.size(); }
        int getNbr(const int d) const { return nbrs_[d]; }

        /**
         * Make sure the node has neighbors before calling this method
         */
        int sampleNbr(randutils::random_generator<>& rng) const {
            auto iter = rng.choose(nbrs_.begin(), nbrs_.end());
            return *iter;
        }

        bool isNbr(const int nbr) const {
            return std::binary_search(nbrs_.begin(), nbrs_.end(), nbr);
        }

        void addNbr(const int nbr) { nbrs_.push_back(nbr); }
        void defrag() {
            std::sort(nbrs_.begin(), nbrs_.end());
            auto last = std::unique(nbrs_.begin(), nbrs_.end());
            nbrs_.erase(last, nbrs_.end());
        }
    };

private:
    mutable randutils::default_rng rng_;   // we don't care rng_
    std::unordered_map<int, Node> nodes_;  // maps a node id to its node object

public:
    UGraph() {}
    virtual ~UGraph() {}

    // disable copy constructor/assignment
    UGraph(const UGraph&) = delete;
    UGraph& operator=(const UGraph&) = delete;

    // move constructor/assignment
    UGraph(UGraph&& other) : nodes_(std::move(other.nodes_)) {
        other.nodes_.clear();
    }

    UGraph& operator=(UGraph&& other) {
        nodes_ = std::move(other.nodes_);
        other.nodes_.clear();
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

    int sampleNode() const {
        auto iter = rng_.choose(nodes_.begin(), nodes_.end());
        return iter->first;
    }

    int sampleNbr(int id) const { return getNode(id).sampleNbr(rng_); }

    void addNode(int id) {
        if (!isNode(id)) nodes_[id] = Node{id};
    }

    /**
     * Add edge (src, dst) to the graph. For the purpose of fast adding,
     * this function does not check whether or not the edge has existed. Callers
     * are responsible to avoid duplidate edges.
     */
    void addEdge(const int src, const int dst) {
        nodes_[src].addNbr(dst);
        if (dst != src) nodes_[dst].addNbr(src);
    }

    /**
     * Optimize the graph data structure for the purpose of fast access.
     * Including: sort neighbors of each node increasingly.
     */
    void optimize() {
        for (auto& p : nodes_) p.second.defrag();
    }
};

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
        int getInNbr(const int d) const { return in_nbrs_[d]; }
        int getOutNbr(const int d) const { return out_nbrs_[d]; }
        bool isInNbr(const int nbr) const {
            return std::binary_search(in_nbrs_.begin(), in_nbrs_.end(), nbr);
        }

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

        void defrag() {
            std::sort(in_nbrs_.begin(), in_nbrs_.end());
            std::sort(out_nbrs_.begin(), out_nbrs_.end());
            auto last_i = std::unique(in_nbrs_.begin(), in_nbrs_.end());
            in_nbrs_.erase(last_i, in_nbrs_.end());
            auto last_o = std::unique(out_nbrs_.begin(), out_nbrs_.end());
            out_nbrs_.erase(last_o, out_nbrs_.end());
        }
    };

private:
    mutable randutils::default_rng rng_;
    std::unordered_map<int, Node> nodes_;  // maps a node id to its node object

public:
    DGraph() {}
    virtual ~DGraph() {}

    // disable copy constructor/assignment
    DGraph(const DGraph&) = delete;
    DGraph& operator=(const DGraph&) = delete;

    // move constructor/assignment
    DGraph(DGraph&& other) : nodes_(std::move(other.nodes_)) {
        other.nodes_.clear();
    }

    DGraph& operator=(DGraph&& other) {
        nodes_ = std::move(other.nodes_);
        other.nodes_.clear();
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
        nodes_[src].addOutNbr(dst);
        nodes_[dst].addInNbr(src);
    }

    /**
     * Optimize the graph data structure for the purpose of fast access.
     * Including: sort neighbors of each node increasingly.
     */
    void optimize() {
        for (auto& p : nodes_) p.second.defrag();
    }
};

template <typename Graph>
Graph loadEdgeList(const std::string& edges_fnm) {
    Graph G;
    ioutils::TSVParser ss{edges_fnm};
    while (ss.next()) {
        int src = ss.get<int>(0), dst = ss.get<int>(1);
        G.addNode(src);
        G.addNode(dst);
        G.addEdge(src, dst);
    }
    G.optimize();
    return G;
}

template <typename Graph>
Graph loadBinEdgeList(const std::string& edges_fnm) {
    Graph G;
    int src, dst;
    auto pin = ioutils::getIOIn(edges_fnm);
    while (!pin->eof()) {
        pin->load(src);
        pin->load(dst);
        G.addNode(src);
        G.addNode(dst);
        G.addEdge(src, dst);
    }
    G.optimize();
    return G;
}

template <typename Graph>
Graph loadBinary(const std::string& filename) {
    Graph G;
    G.load(filename);
    return G;
}

}  // end namespace graph

#endif /* __GRAPH_H__ */
