/**
 * Copyright (C) by J.Z. (07/02/2017 09:09)
 * Distributed under terms of the MIT license.
 */

#ifndef __COMM_H__
#define __COMM_H__

#include <climits>
#include <cassert>

#include <vector>
#include <set>
#include <queue>
#include <stack>
#include <unordered_map>
#include <algorithm>
#include <iterator>

#include "../io/ioutils.h"
#include "../adv/rngutils.h"

namespace graph {

enum class GraphType {
    SIMPLE,  // simple graph
    MULTI,   // multi-graph (two nodes may have multiple edges between them)
};
}

#endif /* __COMM_H__ */
