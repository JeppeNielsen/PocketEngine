//
//  GameSystem.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/12/15.
//  Copyright © 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "metaLib.hpp"
#include "IGameWorld.hpp"
#include "GameObject.hpp"
#include <vector>
#include <map>

template<typename... ComponentList>
class GameSystem {
public:
    
    using Components = meta::list<ComponentList...>;
    virtual ~GameSystem() {};
    
    using Systems = meta::list<>;

public:
    using ObjectCollection = std::vector<GameObject*>;
    ObjectCollection objects;

public:
    const ObjectCollection& Objects() {
        return objects;
    }
    
    using MetaData = std::map<GameObject*, void*>;
    MetaData metadata;
    
    void SetMetaData(GameObject* object, void* data) {
        metadata[object] = data;
    }
    
    void* GetMetaData(GameObject* object) {
        return metadata[object];
    }
};

class GameConcept : public GameSystem<> {

};