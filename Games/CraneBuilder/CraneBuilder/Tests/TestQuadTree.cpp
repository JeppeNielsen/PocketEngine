//
//  TestQuadTree.cpp
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/19/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "TestQuadTree.h"
#include "Quadtree.hpp"

using namespace Pocket;

void TestQuadTree::Test() {

    
    Quadtree tree;
    tree.SetMinMax({0,0}, {100,100});

    typedef std::vector<Quadtree::Node> Nodes;
    Nodes nodes;
    nodes.resize(100);

    int i=0;
    for (int x=0; x<10; x++) {
        for (int y=0; y<10; y++) {
            Quadtree::Node& node = nodes[i++];

            node.data = &node;
            node.min = {x*10.0f,y*10.0f};
            node.max = node.min + Vector2(10.0f,10.0f);
            tree.Insert(node);
        }
    }
    
    typedef std::vector<Quadtree::Node*> Query;
    
    Query query;
    tree.Get({0,0}, {3,3}, query);
    
    std::cout<<"Count: " <<query.size()<<std::endl;

}

