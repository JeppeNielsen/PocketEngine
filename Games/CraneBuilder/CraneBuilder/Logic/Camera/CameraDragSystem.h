//
//  CameraDragSystem.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 07/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Transform.hpp"
#include "CameraDragger.h"
#include "InputManager.hpp"
#include "Camera.hpp"

SYSTEM(CameraDragSystem, Transform, CameraDragger, Camera)
public:
    CameraDragSystem();
    Property<CameraDragSystem*, InputManager*> Input;
    void Update(float dt);
private:
    void InputChanged(Property<CameraDragSystem*, InputManager*>::EventData e);
    void TouchDown(TouchEvent e);
    void TouchUp(TouchEvent e);
    void ScrollChanged(float delta);
};