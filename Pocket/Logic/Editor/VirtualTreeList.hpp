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

namespace Pocket {
    struct VirtualTreeList {
        VirtualTreeList();
        void operator=(const VirtualTreeList& other);
        
        Property<GameObject*> Pivot;
        Property<GameObject*> Root;
        
        struct ExpandedNode {
            DirtyProperty<int> Height;
        };
        
        using ExpandedNodes = std::map<GameObject*, ExpandedNode>;
        ExpandedNodes expandedNodes;
        
        void SetNodeExpanded(GameObject* node, bool expand);
        bool IsNodeExpanded(GameObject* node);
        
        int GetNodeHeight(GameObject* node);
        
        struct Node {
            GameObject* node;
            int position;
            int depth;
            
            inline bool operator ==(const Node &other) const{
                return !(node!=other.node || position!=other.position || depth!=other.depth);
            }

            inline  bool operator !=(const Node &other) const{
                return (node!=other.node || position!=other.position || depth!=other.depth);
            }
            
            inline bool operator < (const Node& other) const {
                if (node!=other.node) return ((void*)node)<((void*)other.node);
                if (position!=other.position) return position<other.position;
                return depth<other.depth;
            }
            
            GameObject* data;
        };
        
        using Nodes = std::vector<Node>;
        Nodes visibleNodes;
        
        Event<Node> NodeCreated;
        Event<Node> NodeRemoved;
      
        void GetNodes_old(int lower, int upper, Nodes& nodesFound);
        
        void GetNodes(int lower, int upper, Nodes& nodesFound);
        
        void GetNodesRecursive(GameObject* object, int lower, int upper, int& index, int depth, Nodes& nodesFound);
        
    };
}