//
//  ScriptTest.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 23/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameSystem.hpp"

using namespace Pocket;

struct Transform {
    int position;
};

struct Velocity {
    int velocity;
};

struct VelocitySystem : public GameSystem<Transform, Velocity> {
    void ObjectAdded(GameObject* object) {
    
    }
    
    void ObjectRemoved(GameObject* object) {
    
    }
};