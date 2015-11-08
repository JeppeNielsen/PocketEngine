//
//  Editor.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 07/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "Engine.hpp"
#include "GameWorld.hpp"

using namespace Pocket;

class Editor : public GameState<Editor> {
public:
    void Initialize();
    void Update(float dt);
    void Render();
private:
    GameWorld world;
};