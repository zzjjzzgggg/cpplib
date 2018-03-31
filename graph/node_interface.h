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

    const int getID() const { return id_; }
    virtual void save(std::unique_ptr<ioutils::IOOut>& po) const = 0;
    virtual void load(std::unique_ptr<ioutils::IOIn>& pi) = 0;

    virtual int getDeg() const = 0;
    virtual int getInDeg() const { return getDeg(); }
    virtual int getOutDeg() const { return getDeg(); }

    virtual int getNbr(const int d) const = 0;
    virtual int getInNbr(const int d) const { return getNbr(d); }
    virtual int getOutNbr(const int d) const { return getNbr(d); }

    virtual bool isNbr(const int nbr) const = 0;
    virtual bool isInNbr(const int nbr) const { return isNbr(nbr); };
    virtual bool isOutNbr(const int nbr) const { return isNbr(nbr); };

    virtual NbrIter beginNbr() const = 0;
    virtual NbrIter endNbr() const = 0;

    virtual NbrIter beginInNbr() const { return beginNbr(); }
    virtual NbrIter endInNbr() const { return endNbr(); }

    virtual NbrIter beginOutNbr() const { return beginNbr(); }
    virtual NbrIter endOutNbr() const { return endNbr(); }

}; /* INode */
}  /* namespace graph */
#endif /* __NODE_INTERFACE_H__ */
