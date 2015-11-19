//
//  Patch.hpp
//  Cubism
//
//  Created by Jeppe Nielsen on 8/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "Block.hpp"
#include "Property.hpp"
#include <string>

using namespace Pocket;

Component(Patch)
public:
    Patch();
    ~Patch();
    void Reset();

    GameObject* block;

    Property<Patch*, GameObject*> BlockAttached;

    std::string name;
};