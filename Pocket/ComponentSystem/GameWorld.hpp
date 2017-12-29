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

    class ScriptWorld;
    class GameObjectHandle;
    
    
    
    class GameWorld {
    private:
        
        GameStorage* storage;
        
        Container<GameScene> scenes;
        std::vector<GameScene*> activeScenes;
        std::vector<GameObject*> roots;
        
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
        
        using Handles = std::vector<GameObjectHandle*>;
        Handles handles;
    public:
        using SceneLayers = std::map<int, GameObject*>;
    private:
        SceneLayers sceneLayers;
        
        void DoActions(Actions &actions);
        void RemoveRoot(GameObject* root);
        GameScene* TryGetScene(const std::string& guid);
        GameObject* FindObject(const std::string& guid, int objectId);
        std::string TryFindScenePath(const std::string& guid);
       
        void AddActiveSystem(IGameSystem* system, GameScene* scene);
        void RemoveActiveSystem(IGameSystem* system);
        void SortActiveSystems();
        
    public:
    
        GameWorld();
        ~GameWorld();
        GameWorld(GameStorage& storage);
        
        void Initialize(GameStorage& storage);
    
        GameObject* CreateRoot();
        GameObject* TryFindRoot(const std::string& guid);
        
        const ObjectCollection& Roots();
        
        void Update(float dt);
        void UpdateRoot(float dt, GameObject* root);
        void UpdateActions();
        void Render();
        
        void DebugSystems();
        
        void Clear();
        
        InputManager& Input();
        
        std::function<GameObject*(const std::string& guid)> GuidToRoot;
        std::function<std::string(const std::string& guid)> GuidToPath;
        std::function<void(std::vector<std::string>& guids, std::vector<std::string>& paths)> GetPaths;
        
        static std::string ReadGuidFromJson(std::istream& jsonStream);
        
        void InvokeChangeToHandles(GameObject* object);
        
        Container<GameScene>& Scenes();
        
        void SetLayerScene(int layerNo, GameObject* scene);
        
        const SceneLayers& GetSceneLayers();
        
        Event<> LayersChanged;
        
        Event<GameObject*> RootCreated;
        Event<GameObject*> RootRemoved;
        
        void SerializeAndRemoveComponents(std::ostream& stream, const SerializePredicate& predicate);
        void DeserializeAndAddComponents(std::istream& jsonStream);
        
    private:
        void TryParseJsonObject(int parent, minijson::istream_context &context, const std::string& componentName,
                                const std::function<void (int, int)>& callback,
                                const std::function<bool (const std::string& componentName)>& componentCallback);
    public:
        friend class GameScene;
        friend class GameObject;
        friend class ScriptWorld;
        friend class GameObjectHandle;
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
    T* GameObject::GetSystem() {
        return static_cast<T*>(scene->systemsIndexed[GameIdHelper::GetSystemID<T>()]);
    }
}
