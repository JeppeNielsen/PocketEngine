//
//  GameWorld.hpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 17/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <functional>
#include <memory>
#include "MetaLibrary.hpp"
#include "Container.hpp"
#include "TypeInfo.hpp"
#include "GameIDHelper.hpp"
#include "GameObject.hpp"
#include "IGameSystem.hpp"
#include "GameScene.hpp"
#include "InputManager.hpp"
#include "GameStorage.hpp"

namespace Pocket {
    class GameStorage;
    class ScriptWorld;
    class GameObjectHandle;
    class Hierarchy;
    
    class GameWorld {
    private:
        GameStorage* storage;
        std::vector<GameObject*> sceneRoots;
        
        struct ActiveSystem {
            IGameSystem* system;
            GameScene* scene;
            
            friend bool operator == ( const ActiveSystem &s1, const ActiveSystem& s2) {
                return s1.system == s2.system;
            }
        };
        
        std::vector<ActiveSystem> activeSystems;
        
        using Actions = std::deque<std::function<void()>>;
        Actions delayedActions;
        
        InputManager input;
        
        void DoActions(Actions& actions);
        void RemoveScene(GameObject* sceneRoot);
        
        void AddActiveSystem(IGameSystem* system, GameScene* scene);
        void RemoveActiveSystem(IGameSystem* system);
        void SortActiveSystems();
        
    public:
    
        GameWorld();
        ~GameWorld();
        GameWorld(GameStorage& storage);
        
        void Initialize(GameStorage& storage);
    
        GameObject* CreateScene();
        GameObject* CreateScene(GameObject* prefab);
        GameObject* FindScene(const std::string& guid);
        
        const ObjectCollection& Scenes();
        
        void Update(float dt);
        void Render();
        
        void Clear();
        
        InputManager& Input();
        
        Event<GameObject*> SceneCreated;
        Event<GameObject*> SceneRemoved;
        
        GameStorage& Storage() const;

        friend class GameScene;
        friend class GameObject;
        friend class ScriptWorld;
        friend class GameObjectHandle;
        friend class Hierarchy;
        friend class GameStorage;
    };
    
    template<typename T>
    T* GameObject::AddComponent() {
        const ComponentId componentId = GameIdHelper::GetComponentID<T>();
        AddComponent(componentId);
        return static_cast<T*>(GetComponent(componentId));
    }

    template<typename T>
    T* GameObject::AddComponent(GameObject* source) {
        const ComponentId componentId = GameIdHelper::GetComponentID<T>();
        AddComponent(componentId, source);
        return static_cast<T*>(GetComponent(componentId));
    }
    
    template<typename T>
    GameObject* GameObject::GetComponentOwner() {
        const ComponentId componentId = GameIdHelper::GetComponentID<T>();
        return GetComponentOwner(componentId);
    }
    
    template<typename T>
    T* GameObject::ReplaceComponent(GameObject* source) {
        const ComponentId componentId = GameIdHelper::GetComponentID<T>();
        ReplaceComponent(componentId, source);
        return static_cast<T*>(source->GetComponent(componentId));
    }
    
    template<typename T>
    T* GameObject::CreateSystem() {
        const SystemId id = GameIdHelper::GetSystemID<T>();
        scene->CreateSystem(id);
        return id<scene->systemsIndexed.size() ? static_cast<T*>(scene->systemsIndexed[id]) : nullptr;
    }
}
