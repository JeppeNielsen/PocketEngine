//
//  Tester.h
//  Cubism
//
//  Created by Jeppe Nielsen on 01/12/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "Engine.hpp"
#include "GameWorld.hpp"
#include "OpenGL.hpp"

using namespace Pocket;

class Tester : public GameState<Tester> {
public:
    void Initialize();
    void Update(float dt);
    void Render();
private:
    GameWorld world;

};
