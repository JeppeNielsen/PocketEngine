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
#include "GameStorage.hpp"

namespace Pocket {
    class GameWorld;
    class IGameSystem;
    class ScriptWorld;
    class GameObjectHandle;
    class GameObjectJsonSerializer;
    class GameScene {
    private:
        friend class GameWorld;
        friend class GameObject;
        friend class std::allocator<GameScene>;
        friend class ScriptWorld;
        friend class GameObjectHandle;
        friend class GameObjectJsonSerializer;
        
        GameWorld* world;
        GameStorage* storage;
        GameObject* root;
        int index;
        std::vector<IGameSystem*> systemsIndexed;
        int idCounter;
        std::string guid;
        Property<float> timeScale;
        Property<bool> updateEnabled;
        Property<bool> renderEnabled;
        
        GameScene();
        ~GameScene();
        GameScene(const GameScene& other);
        void Update(float dt);
        void Render();
        void DestroySystems();
        GameObject* FindObject(int objectId);
        
        IGameSystem* CreateSystem(int systemId);
        void RemoveSystem(int systemId);
        
        void IterateObjects(const std::function<void(GameObject*)>& callback);
        
        std::function<void(GameObject* object)> ObjectCreated;
        std::function<void(GameObject* object)> ObjectRemoved;
        
        std::function<void(GameObject* object, ComponentId componentId)> ComponentCreated;
        std::function<void(GameObject* object, ComponentId componentId)> ComponentRemoved;
        
        GameObject* CreateEmptyObject(GameObject *parent, GameScene* scene, bool assignId);
        
        friend class Container<GameScene>;
    };
}
