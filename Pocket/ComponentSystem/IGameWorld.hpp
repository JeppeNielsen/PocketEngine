//
//  IGameWorld.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 28/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <functional>
#include <iostream>
#include "GameObject.hpp"
#include "IDHelper.hpp"

class IGameWorld {
protected:
    virtual ~IGameWorld() { }
    
    GameObject::Commands commands;
    std::vector<std::function<void*()>> getSystemCommands;
public:
    virtual int ObjectCount() = 0;
    virtual GameObject* GetObject(int index) = 0;
    
    virtual GameObject* CreateObject() = 0;
    virtual GameObject* CreateObject(std::istream &jsonStream, std::function<void(GameObject*)> onCreated) = 0;
    

    template<typename System>
    System& GetSystem() {
        return *((System*)getSystemCommands[IDHelper::GetSystemID<System>()]());
    }
};