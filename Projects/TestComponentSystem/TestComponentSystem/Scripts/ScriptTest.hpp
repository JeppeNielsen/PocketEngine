//
//  ScriptTest.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 23/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameSystem.hpp"

using namespace Pocket;

struct Position {
    Position() : position(0) { }
    int position;
};

struct Velocity {
    Velocity() : velocity(0) { }
    int velocity;
};

struct VelocitySystem : public GameSystem<Position, Velocity> {
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt) ;
};
