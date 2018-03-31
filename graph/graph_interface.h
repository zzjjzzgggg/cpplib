/**
 * Copyright (C) by J.Z. (03/31/2018 10:13)
 * Distributed under terms of the MIT license.
 */

#ifndef __GRAPH_INTERFACE_H__
#define __GRAPH_INTERFACE_H__

#include "node_interface.h"
#include "edge_iter_interface.h"

namespace graph {

template <class NbrIter>
class IGraph {
public:

protected:
    GraphType gtype_;
    mutable rngutils::default_rng rng_;
    std::unordered_map<int, INode<NbrIter>> nodes_;

public:
    IGraph(const GraphType gtype = GraphType::SIMPLE) : gtype_(gtype) {}
    virtual ~IGraph() {}

    IGraph& operator=(IGraph&& other) {
        gtype_ = other.gtype_;
        nodes_ = std::move(other.nodes_);
        return *this;
    }

    virtual void save(const std::string&) const = 0;
    virtual void load(const std::string&) = 0;

    // constant int
    virtual const int getNodes() const { return nodes_.size(); }
    virtual const int getEdges() const = 0;

    // query node
    virtual INode<NbrIter>& getNode(const int id) { return nodes_[id]; }
    virtual const INode<NbrIter>& getNode(const int id) const {
        return nodes_.at(id);
    }
    virtual INode<NbrIter>& operator[](int id) { return nodes_[id]; };
    virtual const INode<NbrIter>& operator[](int id) const {
        return nodes_.at(id);
    };

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
        if (!isNode(id)) nodes_[id] = INode<NbrIter>{id};
    }
    virtual void addEdge(const int, const int) = 0;

    virtual void defrag() = 0;

}; /* IGraph */

} /* namespace graph */
#endif /* __GRAPH_INTERFACE_H__ */
