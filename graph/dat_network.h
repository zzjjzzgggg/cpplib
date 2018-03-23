/**
 * Copyright (C) by J.Z. (03/23/2018 21:30)
 * Distributed under terms of the MIT license.
 */

#ifndef __DAT_NETWORK_H__
#define __DAT_NETWORK_H__

#include "comm.h"

namespace graph {

/**
 * directed data network with node and edge data
 */
template <class NDat, class EDat>
class DatNet {
public:
    class Node {
    private:
        int id_;
        NDat dat_;
        std::forward_list<std::pair<int, EDat>> nbrs_;

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

        // std::pair<int, EDat> getNbr(const int d) const { return nbrs_[d]; }

        std::vector<int>::iterator beginNbr() { return nbrs_.begin(); }
        std::vector<int>::iterator endNbr() { return nbrs_.end(); }

        void addNbr(const int nbr, const EDat& dat) {
            nbrs_.emplace_after(nbrs_.end(), nbr, dat);
        }

        void sort() { std::sort(nbrs_.begin(), nbrs_.end()); }

        void uniq() {
            auto last_o = std::unique(nbrs_.begin(), nbrs_.end());
            nbrs_.erase(last_o, nbrs_.end());
        }
    };
    typedef typename std::unordered_map<int, Node>::const_iterator NodeIter;

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
            while (cur_ != end_ && cur_edge_ >= cur_->second.getDeg()) {
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
        int getDstID() const { return cur_->second.getNbr(cur_edge_).first; }
    };

private:
    GraphType gtype_;
    mutable rngutils::default_rng rng_;
    std::unordered_map<int, Node> nodes_;  // maps a node id to its node object

public:
    DatNet(const GraphType gtype = GraphType::MULTI) : gtype_(gtype) {}
    virtual ~DatNet(){}

    // disable copy constructor/assignment
    DatNet(const DatNet&) = delete;
    DatNet& operator=(const DatNet&) = delete;

    // move constructor/assignment
    DatNet(DatNet&& other)
        : gtype_(other.gtype_), nodes_(std::move(other.nodes_)) {}

    DatNet& operator=(DatNet&& other) {
        gtype_ = other.gtype_;
        nodes_ = std::move(other.nodes_);
        return *this;
    }

    void save(const std::string& filename) const;
    void load(const std::string& filename);

    bool isNode(const int id) const { return nodes_.find(id) != nodes_.end(); }

    const int getNodes() const { return nodes_.size(); }
    const int getEdges() const {
        int edges = 0;
        for (const auto& p : nodes_) edges += p.second.getDeg();
        return edges;
    }

    Node& getNode(const int id) { return nodes_[id]; }
    const Node& getNode(const int id) const { return nodes_.at(id); }
    Node& operator[](int id) { return nodes_[id]; };
    const Node& operator[](int id) const { return nodes_.at(id); };

    void addNode(int id) {
        if (!isNode(id)) nodes_[id] = Node{id};
    }

    /**
     * Add edge (src, dst) to the graph. For efficiency consideration, this
     * function does not check whether or not the edge has existed.
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
     * Find the first node that out degree is nonzero
     */
    EdgeIter beginEI() const {
        auto ni = nodes_.begin();
        while (ni != nodes_.end() && ni->second.getDeg() == 0) ni++;
        return EdgeIter(ni, nodes_.end());
    }
    EdgeIter endEI() const { return EdgeIter(nodes_.end(), nodes_.end()); }

}; /* DatNet */

}  // end namespace graph

#endif /* __DAT_NETWORK_H__ */
