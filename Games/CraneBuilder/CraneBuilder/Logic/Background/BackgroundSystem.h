//
//  BackgroundSystem.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 08/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Background.h"
#include "Mesh.hpp"
#include "Camera.hpp"
using namespace Pocket;

struct BackgroundSystem : public GameSystem<Background, Mesh> {
public:
    GameObject* CameraObject;
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
};
