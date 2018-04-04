/**
 * Copyright (C) by J.Z. (03/31/2018 10:13)
 * Distributed under terms of the MIT license.
 */

#ifndef __GRAPH_INTERFACE_H__
#define __GRAPH_INTERFACE_H__

#include "node_interface.h"
#include "edge_iter_interface.h"

namespace graph {

template <class T, class Node, class NodeIter, class EdgeIter, class NbrIter>
class IGraph {
protected:
    GraphType gtype_;
    mutable rngutils::default_rng rng_;
    std::unordered_map<int, Node> nodes_;

public:
    IGraph(const GraphType gtype = GraphType::SIMPLE) : gtype_(gtype) {}
    virtual ~IGraph() {}

    /**
     * Move constructor
     */
    IGraph(IGraph&& other)
        : gtype_(std::move(other.gtype_)), nodes_(std::move(other.nodes_)) {}

    /**
     * Move assignment
     */
    T& operator=(T&& other) {
        gtype_ = other.gtype_;
        nodes_ = std::move(other.nodes_);
        return *static_cast<T*>(this);
    }

    virtual void save(const std::string&) const = 0;
    virtual void load(const std::string&) = 0;

    // constant int
    virtual const int getNodes() const { return nodes_.size(); }
    virtual const int getEdges() const = 0;

    // query node
    virtual Node& getNode(const int id) { return nodes_[id]; }
    virtual const Node& getNode(const int id) const { return nodes_.at(id); }
    virtual Node& operator[](int id) { return nodes_[id]; };
    virtual const Node& operator[](int id) const { return nodes_.at(id); };

    // bool query
    virtual bool isNode(const int id) const {
        return nodes_.find(id) != nodes_.end();
    }
    virtual bool isEdge(const int src, const int dst) const = 0;

    // sample a node
    virtual int sampleNode() const {
        auto iter = rng_.choose(nodes_.begin(), nodes_.end());
        return iter->first;
    }

    virtual void addNode(int id) {
        if (!isNode(id)) nodes_[id] = Node{id};
    }

    virtual void addEdge(const int, const int) = 0;
    virtual void addEdgeFast(const int, const int) = 0;

    virtual void clear() {
        for (auto& pr : nodes_) pr.second.clear();
        nodes_.clear();
    }

    /**
     * Optimize the graph data structure for the purpose of fast access,
     * including sorting and uniqing neighbors of each node increasingly.
     */
    virtual void defrag() {
        for (auto& p : nodes_) p.second.shrinkAndSort();
        if (gtype_ == GraphType::SIMPLE)
            for (auto& p : nodes_) p.second.uniq();
    }

    // iterators
    virtual NodeIter beginNI() const { return nodes_.begin(); }
    virtual NodeIter endNI() const { return nodes_.end(); }

    /**
     * find the first node that degree is nonzero, and first neighbor <= node
     */
    virtual EdgeIter beginEI() const {
        auto ni = nodes_.begin();
        while (ni != nodes_.end() && ni->second.getDeg() == 0) ni++;
        return EdgeIter(ni, nodes_.end());
    }

    virtual EdgeIter endEI() const {
        return EdgeIter(nodes_.end(), nodes_.end());
    }

}; /* IGraph */

} /* namespace graph */
#endif /* __GRAPH_INTERFACE_H__ */
