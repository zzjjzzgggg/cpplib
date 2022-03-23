/**
 * Copyright (C) by J.Z. (04/02/2018 14:54)
 * Distributed under terms of the MIT license.
 */
#include <cstdio>
#include "../os/osutils.h"
#include "../graph/graph.h"

using namespace graph;

void test_bfs() {
    // std::string gfn = "/dat/workspace/datasets/cit-HepTh_wcc_digraph_mapped.gz";
    std::string gfn="../../../result/test.txt";
    dir::DGraph graph = loadEdgeList<dir::DGraph>(gfn);
    DirBFS<dir::DGraph> bfs(graph);

    int num = 2;
    int max_hop=5;
    std::unordered_set<int> nodes;
    for (int i = 0; i < 10; i++) {
        int nd = graph.sampleNode();

        bfs.doBFS(nd,max_hop);
        int sz = bfs.getBFSTreeSize();
        printf("BFS from a random node %d reaches %d nodes with max hop %d in total.\n", nd,
               sz,max_hop);

        nodes.clear();
        while (nodes.size() < (size_t)num) nodes.insert(graph.sampleNode());
        bfs.doBFS(nodes.begin(), nodes.end(),max_hop);
        sz = bfs.getBFSTreeSize();
        printf("BFS from random %d nodes reaches %d nodes with max hop %d in total.\n", num,
               sz,max_hop);
    }
}

void test_rev_bfs(){
    std::string gfn="../../../result/test.txt";
    dir::DGraph graph=loadEdgeList<dir::DGraph>(gfn);
    DirBFS<dir::DGraph> rev_bfs(graph);

    int num=2;
    int max_hop=2;
    std::unordered_set<int> nodes;
    for(int i=0;i<10;i++){
        int nd=graph.sampleNode();

        rev_bfs.doRevBFS(nd,max_hop);
        int sz=rev_bfs.getBFSTreeSize();
        printf("Reverse BFS from a random node %d reaches %d nodes with max hop %d in total.\n",nd,sz,max_hop);

        nodes.clear();
        while (nodes.size()<(size_t)num) nodes.insert(graph.sampleNode());
        rev_bfs.doRevBFS(nodes.begin(),nodes.end(),max_hop);
        sz=rev_bfs.getBFSTreeSize();

        printf("Reverse BFS from random %d nodes reaches %d nodes with max hop in %d total.\n",num,sz,max_hop);
    }
}

void test_inc_bfs(){
    std::string gfn="../../../result/test.txt";
    dir::DGraph graph=loadEdgeList<dir::DGraph>(gfn);
    DirBFS<dir::DGraph> inc_bfs(graph);

    int num=2;
    int max_hop=3;
    int nd;
    std::unordered_set<int> nodes={2};
    for(int i=0;i<10;i++){
        int nd=graph.sampleNode();
        
        while (nodes.size()<(size_t)num){
            int node=graph.sampleNode();
            if(node!=nd)
                nodes.insert(node);
        }
        auto nd_to_hop=inc_bfs.doIncBFS(nodes.begin(),nodes.end(),nd,max_hop);
        int sz=nodes.size();
       
        int inc=nd_to_hop.size();
        nodes.clear();
        printf("Do increment BFS for a seed node %d and other %d nodes with max hop %d ,get increment for %d in total.\n",nd,sz,max_hop,inc);
    }
}


int main(int argc, char* argv[]) {
    osutils::Timer tm;

    test_bfs();

    test_rev_bfs();

    test_inc_bfs();

    printf("cost time %s\n", tm.getStr().c_str());
    return 0;
}
