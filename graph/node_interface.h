/**
 * Copyright (C) by J.Z. (03/30/2018 22:05)
 * Distributed under terms of the MIT license.
 */

#ifndef __NODE_INTERFACE_H__
#define __NODE_INTERFACE_H__

class INode {
protected:
    int id_;

public:
    INode();
    virtual ~INode();

    int getID() { return id_; }

}; /* INode */


#endif /* __NODE_INTERFACE_H__ */
