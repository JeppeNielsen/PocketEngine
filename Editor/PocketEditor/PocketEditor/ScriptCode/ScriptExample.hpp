//
//  ScriptExample.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include <iostream>

struct Sprite {
    
    Sprite() : width(100), height(200) {}
    
    float width;
    float height;
};

struct Jumpable {
    
    Jumpable() : JumpHeight(765) {}
    
    int JumpHeight;  
    
};

struct SpriteSystem : GameSystem<Pocket::Transform, Sprite> {
    void ObjectAdded(GameObject* object) {
        std::cout << "SpriteSystem::ObjectAdded"<<std::endl;
    }

    void ObjectRemoved(GameObject* object) {
        std::cout << "SpriteSystem::ObjectRemoved"<<std::endl;
    }
    
    void Update(float dt) {
        for(auto o : Objects()) {
            auto var = o->GetComponent<Pocket::Transform>();
            var->Position += {dt*50,dt,0};
        }
    }
};

struct ScriptTransformSystem : GameSystem<Pocket::Transform, Pocket::Sizeable> {
    
    void ObjectAdded(GameObject* object) {
        std::cout << "ScriptTransformSystem::ObjectAdded"<<std::endl;
    }
        
    void ObjectRemoved(GameObject* object) {
        std::cout << "ScriptTransformSystem::ObjectRemoved"<<std::endl;
    }

    void Update(float dt) {
       for(auto o : Objects()) {
          auto s = o->GetComponent<Pocket::Sizeable>()->Size();
          s.x = o->GetComponent<Pocket::Transform>()->Position().x;
          o->GetComponent<Pocket::Sizeable>()->Size = s;
       }
       //std::cout << Objects()[0]->GetComponent<Sprite>()->width<<std::endl;
    }
};

struct JumpSystem : GameSystem<Pocket::Transform, Jumpable> {
    
    void ObjectAdded(GameObject* object) {
        std::cout << "JumpSystem::ObjectAdded : JumpHeight "<< object->GetComponent<Jumpable>()->JumpHeight <<std::endl;
        object->GetComponent<Jumpable>()->JumpHeight = 233;
        std::cout << "JumpSystem::ObjectAdded : JumpHeight "<< object->GetComponent<Jumpable>()->JumpHeight <<std::endl;
    }
        
    void ObjectRemoved(GameObject* object) {
        std::cout << "JumpSystem::ObjectRemoved"<<std::endl;
    }

    void Update(float dt) {
       //std::cout << Objects()[0]->GetComponent<Sprite>()->width<<std::endl;
    }
    

};