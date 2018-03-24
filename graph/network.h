/**
 * Copyright (C) by J.Z. (03/23/2018 21:30)
 * Distributed under terms of the MIT license.
 */

#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "comm.h"

namespace graph {

/**
 * Directed network allowing node and edge to store data. Use an adjacency list
 * data structure. For each node, only its out neighbors are stored and sorted
 * by function Cmp. For example, Cmp could be std::less<int>
 */
template <class NDat, class EDat, class Cmp>
class DatNet {
public:
    class Node;
    typedef
        typename std::set<std::pair<int, EDat>, Cmp>::const_iterator NbrIter;
    typedef typename std::unordered_map<int, Node>::const_iterator NodeIter;

public:
    /**
     * A node object stores its ID, data, and out-neighbors
     */
    class Node {
    private:
        int id_;
        NDat dat_;
        std::set<std::pair<int, EDat>, Cmp> out_nbrs_;

    public:
        Node() : id_(-1) {}
        Node(const int id) : id_(id) {}
        Node(const int id, const NDat& dat) : id_(id), dat_(dat) {}

        // disable copy constructor/assignment
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;

        // move constructor/assignment
        Node(Node&& other)
            : id_(other.id_), out_nbrs_(std::move(other.out_nbrs_)) {}

        Node& operator=(Node&& other) {
            id_ = other.id_;
            out_nbrs_ = std::move(other.out_nbrs_);
            return *this;
        }

        // only structure is saved
        void save(std::unique_ptr<ioutils::IOOut>& po) const {
            po->save(id_);                            // id
            po->save((int)out_nbrs_.size());          // out-deg
            for (int nbr : out_nbrs_) po->save(nbr);  // out-neighbors
        }
        void load(std::unique_ptr<ioutils::IOIn>& pi) {
            int deg, nbr;
            pi->load(id_);           // id
            pi->load(deg);           // out-deg
            out_nbrs_.reserve(deg);  // out-neighbors
            for (int d = 0; d < deg; d++) {
                pi->load(nbr);
                out_nbrs_.push_back(nbr);
            }
        }

        int getID() const { return id_; }
        int getOutDeg() const { return out_nbrs_.size(); }

        // std::pair<int, EDat>& getNbr(const int d) const { return
        // out_nbrs_[d]; }

        NbrIter beginOutNbr() const { return out_nbrs_.begin(); }
        NbrIter endOutNbr() const { return out_nbrs_.end(); }

        void addOutNbr(const int nbr, const EDat& dat) {
            out_nbrs_.emplace(nbr, dat);
        }
    };

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
    class EdgeIter {
    private:
        NodeIter cur_nd_, end_nd_;
        NbrIter cur_edge_;

    public:
        EdgeIter() {}
        EdgeIter(const NodeIter& start_nd_iter, const NodeIter& end_nd_iter)
            : cur_nd_(start_nd_iter), end_nd_(end_nd_iter) {
            if (cur_nd_ != end_nd_) cur_edge_ = cur_nd_->second.beginOutNbr();
        }

        // copy assignment
        EdgeIter& operator=(const EdgeIter& ei) {
            if (this != &ei) {
                cur_nd_ = ei.cur_nd_;
                end_nd_ = ei.end_nd_;
                cur_edge_ = ei.cur_edge_;
            }
            return *this;
        }

        // postfix increment: i++
        // move to next valid edge; if not exists, move to end
        EdgeIter& operator++(int) {
            cur_edge_++;
            while (cur_edge_ == cur_nd_->second.endOutNbr()) {
                cur_nd_++;
                if (cur_nd_ == end_nd_) break;
                cur_edge_ = cur_nd_->second.beginOutNbr();
            }
            return *this;
        }

        bool operator!=(const EdgeIter& ei) const {
            return cur_nd_ != ei.cur_nd_ || end_nd_ != ei.end_nd_ ||
                   (cur_nd_ != end_nd_ && cur_edge_ != ei.cur_edge_);
        }

        /**
         * If both two node iterators point to end, return true;
         * Otherwise, compare the two attribute by attribute.
         */
        bool operator==(const EdgeIter& ei) const {
            return cur_nd_ == ei.cur_nd_ && end_nd_ == ei.end_nd_ &&
                   (cur_nd_ == end_nd_ || cur_edge_ == ei.cur_edge_);
        }

        int getSrcID() const { return cur_nd_->second.getID(); }
        int getDstID() const { return cur_edge_->first; }
        const EDat& getDat() const { return cur_edge_->second; }
    };

private:
    mutable rngutils::default_rng rng_;
    std::unordered_map<int, Node> nodes_;  // maps a node id to its node object

public:
    DatNet() {}
    virtual ~DatNet() {}

    // disable copy constructor/assignment
    DatNet(const DatNet&) = delete;
    DatNet& operator=(const DatNet&) = delete;

    // move constructor/assignment
    DatNet(DatNet&& other) : nodes_(std::move(other.nodes_)) {}

    DatNet& operator=(DatNet&& other) {
        nodes_ = std::move(other.nodes_);
        return *this;
    }

    void save(const std::string& filename) const {
        auto po = ioutils::getIOOut(filename);
        po->save((int)nodes_.size());                // total number of nodes
        for (auto& pr : nodes_) pr.second.save(po);  // each node
    }
    void load(const std::string& filename) {
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

    bool isNode(const int id) const { return nodes_.find(id) != nodes_.end(); }

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

    void addNode(const int id) {
        if (!isNode(id)) nodes_[id] = Node{id};
    }

    void addNode(const int id, const NDat& dat) {
        if (!isNode(id)) nodes_[id] = Node{id, dat};
    }

    /**
     * Add edge (src, dst, dat) to the graph.
     */
    void addEdge(const int src, const int dst, const EDat& dat) {
        addNode(src);
        addNode(dst);
        nodes_[src].addOutNbr(dst, dat);
    }

    // iterators
    NodeIter beginNI() const { return nodes_.begin(); }
    NodeIter endNI() const { return nodes_.end(); }
    EdgeIter beginEI() const {
        // Find the first node such that its out degree is nonzero
        auto ni = nodes_.begin();
        while (ni != nodes_.end() && ni->second.getOutDeg() == 0) ni++;
        return EdgeIter(ni, nodes_.end());
    }
    EdgeIter endEI() const { return EdgeIter(nodes_.end(), nodes_.end()); }

}; /* DatNet */

}  // end namespace graph

#endif /* __NETWORK_H__ */
