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

struct CameraDragSystem: public GameSystem<Transform, CameraDragger, Camera> {
public:
    CameraDragSystem();
    Property<InputManager*> Input;
    void Update(float dt);
private:
    void InputChanged();
    void TouchDown(TouchEvent e);
    void TouchUp(TouchEvent e);
    void ScrollChanged(float delta);
};