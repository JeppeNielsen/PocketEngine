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
        VirtualTreeListSpawner();
        std::function<GameObject*(GameObject*, GameObject*)> OnCreate;
        std::function<void(GameObject*, GameObject*)> OnRemove;
        std::vector<VirtualTreeList::Node> objects;
        std::function<bool(GameObject*)> HasChildren;
  };
}
