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
    Position() : position(0) { }
    int position;
};

struct Velocity {
    Velocity() : velocity(0) { }
    int velocity;
};

struct VelocitySystem : public GameSystem<Position, Velocity> {
    void ObjectAdded(GameObject* object) {
        std::cout << "Object added "<< (this) <<  std::endl;
    }
    
    void ObjectRemoved(GameObject* object) {
        std::cout << "Object removed "<< std::endl;
    }
    
    void Update(float dt) {
        for(auto object : Objects()) {
            auto pos = object->GetComponent<Position>();
            auto vel = object->GetComponent<Velocity>();
            
            pos->position += vel->velocity;
        }
    }
};