//
//  CameraSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 4/21/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "CameraSystem.hpp"
#include "Transform.hpp"
#include "Camera.hpp"

using namespace Pocket;

void CameraSystem::Initialize() {
    AddComponent<Transform>();
    AddComponent<Camera>();
}
