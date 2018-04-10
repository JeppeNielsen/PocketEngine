//
//  VirtualTreeList.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 28/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include "DirtyProperty.hpp"
#include "GameObject.hpp"
#include <vector>
#include <map>

namespace Pocket {
    struct VirtualTreeList {
        VirtualTreeList();
        void operator=(const VirtualTreeList& other);
        
        Property<GameObject*> Pivot;
        Property<GameObject*> Root;
        Property<float> ItemHeight;
        Property<float> ItemIndent;
        Property<Vector2> Margins;
        
        std::function<std::string(GameObject*)> ExpandedHashFunction;
        std::function<bool(GameObject* object)> PredicateFunction;
        
        struct ExpandedNode {
            DirtyProperty<int> Height;
        };
        
        using ExpandedNodes = std::map<std::string, ExpandedNode>;
        ExpandedNodes expandedNodes;
        
        void SetNodeExpanded(GameObject* node, bool expand);
        bool IsNodeExpanded(GameObject* node);
        
        int GetNodeHeight(GameObject* node);
        
        std::string DefaultExpandedHashFunction(GameObject* go);
        
        struct Node {
            GameObject* node;
            int position;
            int depth;
            bool hasChildren;

            inline bool operator ==(const Node &other) const{
                return !(node!=other.node || position!=other.position || depth!=other.depth || hasChildren!=other.hasChildren);
            }

            inline  bool operator !=(const Node &other) const{
                return (node!=other.node || position!=other.position || depth!=other.depth || hasChildren!=other.hasChildren);
            }
            
            inline bool operator < (const Node& other) const {
                if (node!=other.node) return ((void*)node)<((void*)other.node);
                if (position!=other.position) return position<other.position;
                return depth<other.depth;
            }
        };
        
        using Nodes = std::vector<Node>;
        Nodes visibleNodes;
        
        Event<Node> NodeCreated;
        Event<Node> NodeRemoved;
        Event<Node> NodeExpanded;
        
        void GetNodes(int lower, int upper, Nodes& nodesFound);
        
        void GetNodesRecursive(GameObject* object, int lower, int upper, int& index, int depth, Nodes& nodesFound);
        
        void Clear();
        
        bool ShowRoot;
    };
}
