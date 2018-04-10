//
//  VirtualTreeList.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 28/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "VirtualTreeList.hpp"
#include <sstream>

using namespace Pocket;

VirtualTreeList::VirtualTreeList() {
    Root = 0;
    ExpandedHashFunction = [this] (GameObject* go) {
        return DefaultExpandedHashFunction(go);
    };
    PredicateFunction = 0;
    ShowRoot = false;
    ItemHeight = 25;
    ItemIndent = 25;
    Margins = 2;
}

void VirtualTreeList::operator=(const Pocket::VirtualTreeList &other) {
    Root = 0;
    Pivot = 0;
    visibleNodes.clear();
    expandedNodes.clear();
    NodeCreated.Clear();
    NodeRemoved.Clear();
}

void VirtualTreeList::SetNodeExpanded(Pocket::GameObject *node, bool expand) {
    std::string hash = ExpandedHashFunction(node);
    auto it = expandedNodes.find(hash);
    if (expand && it==expandedNodes.end()) {
        ExpandedNode& expandedNode = expandedNodes[hash];
        expandedNode.Height.Method = [this, node](auto& v) {
            v = 1;
            for(auto o : node->Hierarchy().Children()) {
                v += this->GetNodeHeight(o);
            }
        };
        expandedNode.Height.MakeDirty();
        if (node->Hierarchy().Parent()) {
            expandedNodes[ExpandedHashFunction(node->Hierarchy().Parent())].Height.MakeDirty();
        }
    } else if (!expand && it!=expandedNodes.end()) {
        expandedNodes.erase(it);
        if (node->Hierarchy().Parent()) {
            expandedNodes[ExpandedHashFunction(node->Hierarchy().Parent())].Height.MakeDirty();
        }
    }
    
    for(auto& n : visibleNodes) {
        if (n.node == node) {
            NodeExpanded(n);
            break;
        }
    }
}

bool VirtualTreeList::IsNodeExpanded(Pocket::GameObject *node) {
    if (!ShowRoot && node == Root()) return true;
    std::string hash = ExpandedHashFunction(node);
    return expandedNodes.find(hash) != expandedNodes.end();
}

int VirtualTreeList::GetNodeHeight(Pocket::GameObject *node) {
    if (!ShowRoot && node == Root) {
        int v = 1;
        for(auto o : node->Hierarchy().Children()) {
            v += this->GetNodeHeight(o);
        }
        return v;
    }

    std::string hash = ExpandedHashFunction(node);
    auto it = expandedNodes.find(hash);
    return it!=expandedNodes.end() ? it->second.Height() : 1;
}

void VirtualTreeList::GetNodes(int lower, int upper, Nodes &nodesFound) {
    int index = 0;
    GetNodesRecursive(Root, lower, upper, index, ShowRoot ? 0 : -1, nodesFound);
}

void VirtualTreeList::GetNodesRecursive(Pocket::GameObject *object, int lower, int upper, int &index, int depth, Nodes &nodesFound) {
    if (index>upper) return;
    if ((ShowRoot || (object!=Root)) && PredicateFunction && !PredicateFunction(object)) return;
    if (index>=lower && index<=upper) {
        if (ShowRoot || (object!=Root)) {
            nodesFound.push_back({object, index, depth, !object->Hierarchy().Children().empty() });
        }
    }
    index++;
    
    int height = GetNodeHeight(object);
    
    if (height>1) {
        int endIndex = height + index-1;
        if (!(upper<index || lower>=endIndex)) {
            for(auto child : object->Hierarchy().Children()) {
                GetNodesRecursive(child, lower, upper, index, depth+1, nodesFound);
            }
        } else {
            index=endIndex;
        }
    }
}

std::string VirtualTreeList::DefaultExpandedHashFunction(Pocket::GameObject *go) {
    std::stringstream s;
    s<<go;
    return s.str();
}

void VirtualTreeList::Clear() {
    for(auto& node : visibleNodes) {
        NodeRemoved({ node.node, node.position, node.depth });
    }
    visibleNodes.clear();
    expandedNodes.clear();
}


