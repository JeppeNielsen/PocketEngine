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

struct SizeModifierNode {
    SizeModifierNode();
    int cornerIndex;
    Transform* transformTarget;
    Sizeable* sizableTarget;
    Vector2 GetCornerPosition();
};
