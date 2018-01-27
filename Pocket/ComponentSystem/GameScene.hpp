//
//  GameScene.hpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 18/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <functional>
#include "GameObject.hpp"
#include "Property.hpp"

namespace Pocket {
    class GameWorld;
    class IGameSystem;
    class ScriptWorld;
    class GameObjectHandle;
    class GameObjectJsonSerializer;
    class GameStorage;
    class Hierarchy;
    
    class GameScene {
    private:
        friend class GameWorld;
        friend class GameObject;
        friend class std::allocator<GameScene>;
        friend class ScriptWorld;
        friend class GameObjectHandle;
        friend class GameObjectJsonSerializer;
        friend class GameStorage;
        friend class Hierarchy;
        
        std::string guid;
        GameWorld* world;
        GameStorage* storage;
        GameObject* root;
        std::vector<IGameSystem*> systemsIndexed;
        int idCounter;
        int index;
        
        GameScene();
        ~GameScene();
        
        void Initialize(GameWorld* world, GameStorage* storage, int index);
        void Destroy();
        
        void CreateSystem(int systemId);
        void RemoveSystem(int systemId);
        
        void IterateObjects(const std::function<void(GameObject*)>& callback);
        
        std::function<void(GameObject* object)> ObjectCreated;
        std::function<void(GameObject* object)> ObjectRemoved;
        
        std::function<void(GameObject* object, ComponentId componentId)> ComponentCreated;
        std::function<void(GameObject* object, ComponentId componentId)> ComponentRemoved;
        
        GameObject* CreateEmptyObject(GameObject *parent, bool assignId);
        
        friend class Container<GameScene>;
        
        GameObject* FindObject(int objectId);
    };
}
