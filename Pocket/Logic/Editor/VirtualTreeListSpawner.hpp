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
        std::function<GameObject*(GameObject*, GameObject*)> OnCreate;
        std::vector<VirtualTreeList::Node> objects;
  };
}