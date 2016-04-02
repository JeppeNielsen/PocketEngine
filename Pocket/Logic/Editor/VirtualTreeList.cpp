//
//  VirtualTreeList.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 28/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "VirtualTreeList.hpp"

using namespace Pocket;

VirtualTreeList::VirtualTreeList() {
    Root = 0;
}

void VirtualTreeList::operator=(const Pocket::VirtualTreeList &other) {
    Root = 0;
}

void VirtualTreeList::SetNodeExpanded(Pocket::GameObject *node, bool expand) {
    auto it = expandedNodes.find(node);
    if (expand && it==expandedNodes.end()) {
        ExpandedNode& expandedNode = expandedNodes[node];
        expandedNode.Height.Method = [this, node](auto& v) {
            v = 1;
            for(auto o : node->Children()) {
                v += this->GetNodeHeight(o);
            }
        };
        expandedNode.Height.MakeDirty();
        if (node->Parent()) {
            expandedNodes[node->Parent()].Height.MakeDirty();
        }
    } else if (!expand && it!=expandedNodes.end()) {
        expandedNodes.erase(it);
        if (node->Parent()) {
            expandedNodes[node->Parent()].Height.MakeDirty();
        }
    }
}

bool VirtualTreeList::IsNodeExpanded(Pocket::GameObject *node) {
    return expandedNodes.find(node) != expandedNodes.end();
}

int VirtualTreeList::GetNodeHeight(Pocket::GameObject *node) {
    auto it = expandedNodes.find(node);
    return it!=expandedNodes.end() ? it->second.Height() : 1;
}

void VirtualTreeList::GetNodes(int lower, int upper, Nodes &nodesFound) {
    int index = 0;
    GetNodesRecursive(Root, lower, upper, index, 0, nodesFound);
}

void VirtualTreeList::GetNodesRecursive(Pocket::GameObject *object, int lower, int upper, int &index, int depth, Nodes &nodesFound) {
    if (index>upper) return;

    if (index>=lower && index<=upper) {
        nodesFound.push_back({object, index, depth });
    }
    index++;
    
    int height = GetNodeHeight(object);
    
    if (height>1) {
        int endIndex = height + index-1;
        if (!(upper<index || lower>=endIndex)) {
            for(auto child : object->Children()) {
                GetNodesRecursive(child, lower, upper, index, depth+1, nodesFound);
            }
        } else {
            index=endIndex;
        }
    }
}



