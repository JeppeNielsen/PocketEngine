//
//  SizeModifierLineSystem.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 15/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "SizeModifierLine.h"
#include "Transform.hpp"

using namespace Pocket;

SYSTEM(SizeModifierLineSystem, Transform, SizeModifierLine)
    void Update(float dt);
};