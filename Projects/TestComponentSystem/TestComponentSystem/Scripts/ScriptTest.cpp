//
//  ScriptTest.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 23/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameSystem.hpp"
#include <iostream>

using namespace Pocket;

struct Position {
    int position;
};

struct Velocity {
    int velocity;
};

struct VelocitySystem : public GameSystem<Position, Velocity> {
    void ObjectAdded(GameObject* object) {
        std::cout << "Object added "<< std::endl;
    }
    
    void ObjectRemoved(GameObject* object) {
        std::cout << "Object removed "<< std::endl;
    }
};