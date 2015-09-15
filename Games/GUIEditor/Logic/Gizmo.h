//
//  Gizmo.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 14/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"

using namespace Pocket;

Component(Gizmo)
public:
    void Reset();
    GameObject* corners[8];

};


