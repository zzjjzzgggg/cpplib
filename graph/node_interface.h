/**
 * Copyright (C) by J.Z. (03/30/2018 22:05)
 * Distributed under terms of the MIT license.
 */

#ifndef __NODE_INTERFACE_H__
#define __NODE_INTERFACE_H__

#include "comm.h"

namespace graph {
template <class NbrIter>
class INode {
protected:
    int id_;

public:
    INode(int id) : id_(id) {}
    virtual ~INode() {}

    INode(const INode& o) : id_(o.id_) {}
    INode(INode&& o) : id_(o.id_) {}

    INode& operator=(const INode& o) {
        id_ = o.id_;
        return *this;
    }
    INode& operator=(INode&& o) {
        id_ = o.id_;
        return *this;
    }

    const int getID() const { return id_; }
    virtual void save(std::unique_ptr<ioutils::IOOut>& po) const = 0;
    virtual void load(std::unique_ptr<ioutils::IOIn>& pi) = 0;

    virtual int getDeg() const = 0;
    virtual int getInDeg() const { return getDeg(); }
    virtual int getOutDeg() const { return getDeg(); }

    virtual int getNbrID(const NbrIter&) const = 0;

    virtual bool isNbr(const int nbr) const = 0;
    virtual bool isInNbr(const int nbr) const { return isNbr(nbr); };
    virtual bool isOutNbr(const int nbr) const { return isNbr(nbr); };

    virtual void addNbr(const int nbr) = 0;
    virtual void addInNbr(const int nbr) { addNbr(nbr); }
    virtual void addOutNbr(const int nbr) { addNbr(nbr); }

    virtual void addNbrFast(const int nbr) = 0;
    virtual void addInNbrFast(const int nbr) { addNbrFast(nbr); }
    virtual void addOutNbrFast(const int nbr) { addNbrFast(nbr); }

    virtual void clear() = 0;

    // The following three iterators are designed to iterate over both in and
    // out neighbors of a node.
    virtual NbrIter beginNbr() const = 0;
    virtual NbrIter endNbr() const = 0;
    virtual void nextNbr(NbrIter& ni) const { ni++; };

    virtual NbrIter beginInNbr() const { return beginNbr(); }
    virtual NbrIter endInNbr() const { return endNbr(); }

    virtual NbrIter beginOutNbr() const { return beginNbr(); }
    virtual NbrIter endOutNbr() const { return endNbr(); }

}; /* INode */
} /* namespace graph */
#endif /* __NODE_INTERFACE_H__ */
