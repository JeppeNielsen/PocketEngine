//
//  VirtualTreeListSpawner.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 29/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <map>
#include <functional>
#include "GameObject.hpp"
#include "VirtualTreeList.hpp"

namespace Pocket {
  class VirtualTreeListSpawner {
    public:
      
        struct SpawnedNode {
            GameObject* node;
            GameObject* parent;
            bool hasChildren;
            float height;
            GameObject* foldOutButton;
        };
    
        VirtualTreeListSpawner();
        std::function<void(SpawnedNode& node)> OnCreate;
        std::function<void(const SpawnedNode& node)> OnRemove;
        std::map<VirtualTreeList::Node, SpawnedNode> objects;
        std::function<bool(GameObject*)> HasChildren;
  };
}
