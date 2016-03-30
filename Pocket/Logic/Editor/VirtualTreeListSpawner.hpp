//
//  VirtualTreeListSpawner.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 29/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <map>
#include "GameObject.hpp"
#include "VirtualTreeList.hpp"

namespace Pocket {
  class VirtualTreeListSpawner {
    public:
        GameObject* prefab;
        std::vector<VirtualTreeList::Node> objects;
  };
}