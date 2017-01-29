//
//  VirtualTreeListSpawner.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 29/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "VirtualTreeListSpawner.hpp"

using namespace Pocket;

VirtualTreeListSpawner::VirtualTreeListSpawner() {
    OnCreate = 0;
    OnRemove = 0;
    HasChildren = 0;
}
