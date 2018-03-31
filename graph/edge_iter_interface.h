/**
 * Copyright (C) by J.Z. (03/30/2018 22:33)
 * Distributed under terms of the MIT license.
 */

#ifndef __EDGE_ITER_INTERFACE_H__
#define __EDGE_ITER_INTERFACE_H__

#include "comm.h"

namespace graph {
template <class T, class NodeIter, class NbrIter>
class IEdgeIter {
protected:
    NodeIter cur_nd_, end_nd_;
    NbrIter cur_edge_;

public:
    IEdgeIter() {}
    IEdgeIter(const NodeIter& start_nd_iter, const NodeIter& end_nd_iter)
        : cur_nd_(start_nd_iter), end_nd_(end_nd_iter) {
        if (cur_nd_ != end_nd_) cur_edge_ = cur_nd_->second.beginOutNbr();
    }
    virtual ~IEdgeIter() {}

    // copy assignment
    T& operator=(const T& edge_iter) {
        if (this != &edge_iter) {
            cur_nd_ = edge_iter.cur_nd_;
            end_nd_ = edge_iter.end_nd_;
            cur_edge_ = edge_iter.cur_edge_;
        }
        return *static_cast<T*>(this);
    }

    // postfix increment: i++
    // move to next valid edge; if not exists, move to end
    T& operator++(int) {
        cur_edge_++;
        while (cur_edge_ == cur_nd_->second.endOutNbr()) {
            cur_nd_++;
            if (cur_nd_ == end_nd_) break;
            cur_edge_ = cur_nd_->second.beginOutNbr();
        }
        return *static_cast<T*>(this);
    }

    bool operator!=(const T& ei) const {
        return cur_nd_ != ei.cur_nd_ || end_nd_ != ei.end_nd_ ||
               (cur_nd_ != end_nd_ && cur_edge_ != ei.cur_edge_);
    }

    /**
     * If both two node iterators point to end, return true;
     * Otherwise, compare the two attribute by attribute.
     */
    inline bool operator==(const T& ei) const {
        return cur_nd_ == ei.cur_nd_ && end_nd_ == ei.end_nd_ &&
               (cur_nd_ == end_nd_ || cur_edge_ == ei.cur_edge_);
    }

    virtual int getSrcID() const = 0;
    virtual int getDstID() const = 0;

}; /* IEdgeIter */
}  /* namespace graph */

#endif /* __EDGE_ITER_INTERFACE_H__ */
