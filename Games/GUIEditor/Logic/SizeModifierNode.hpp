//
//  SizeModifierNode.hpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 3/22/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Vector2.hpp"
#include "Transform.hpp"
#include "Sizeable.hpp"

using namespace Pocket;

Component(SizeModifierNode)
public:
    int cornerIndex;
    Transform* transformTarget;
    Sizeable* sizableTarget;
    Vector2 GetCornerPosition();

    void Reset();
};
