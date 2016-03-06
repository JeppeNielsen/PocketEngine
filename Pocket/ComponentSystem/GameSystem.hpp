//
//  GameSystem.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/12/15.
//  Copyright © 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <tuple>
#include <vector>
#include <map>
#include "GameConstants.hpp"

class GameWorld;
class GameObject;

class IGameSystem {
protected:
    virtual ~IGameSystem();
    
    using ObjectCollection = std::vector<GameObject*>;
    ObjectCollection objects;

public:
    const ObjectCollection& Objects();
protected:
    using MetaData = std::map<GameObject*, void*>;
    MetaData metadata;
    
    void SetMetaData(GameObject* object, void* data);
    void* GetMetaData(GameObject* object);
    
    virtual void Initialize(GameWorld* world);
    virtual void Update(float dt);
    virtual void Render();
    virtual void ObjectAdded(GameObject* object);
    virtual void ObjectRemoved(GameObject* object);
    virtual void CreateComponents(GameWorld *world, int systemIndex) = 0;
    
    int index;
    
    friend class GameObject;
    friend class GameWorld;
};

template<typename... ComponentList>
class GameSystem : public IGameSystem {
protected:
    GameSystem() { }
    virtual ~GameSystem() { }
    
    void CreateComponents(GameWorld *world, int systemIndex) override;
};

class GameConcept : public GameSystem<> {

};