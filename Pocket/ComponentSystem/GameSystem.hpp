//
//  GameSystem.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/12/15.
//  Copyright © 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "metaLib.hpp"
#include <vector>
#include <map>

template<typename T>
class GameObject;

template<typename T>
class GameWorld;

template<typename T, typename... ComponentList>
class GameSystem {
public:
    friend class GameObject<T>;
    friend class GameWorld<T>;

    using GameObject = GameObject<T>;
    
    using Components = meta::list<ComponentList...>;
    virtual ~GameSystem() {};
    
    using Systems = meta::list<>;
    
    using ObjectCollection = std::vector<GameObject*>;

private:
    
    ObjectCollection objects;
    GameWorld<T>* world;
    
public:
    const ObjectCollection& Objects() {
        return objects;
    }

    GameWorld<T>& World() {
        return *world;
    }
    
    using MetaData = std::map<GameObject*, void*>;
    
    MetaData metaData;
    
    void SetMetaData(GameObject* object, void* data) {
        metaData[object] = data;
    }
    
    void* GetMetaData(GameObject* object) {
        return metaData[object];
    }
};