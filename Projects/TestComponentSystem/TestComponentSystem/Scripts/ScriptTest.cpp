//
//  ScriptTest.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 23/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "ScriptTest.hpp"
#include <iostream>

using namespace Pocket;
void VelocitySystem::ObjectAdded(GameObject* object) {
    std::cout << "Object added "<< (this) <<  std::endl;
}

void VelocitySystem::ObjectRemoved(GameObject* object) {
    std::cout << "Object removed "<< std::endl;
}

void VelocitySystem::Update(float dt) {
    for(auto object : Objects()) {
        auto pos = object->GetComponent<Position>();
        auto vel = object->GetComponent<Velocity>();
        
        pos->position += vel->velocity;
    }
}
