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
#include "GameIdHelper.hpp"
#include "GameObject.hpp"
#include "IGameSystem.hpp"
#include "GameScene.hpp"
#include "InputManager.hpp"

namespace Pocket {

    class ScriptWorld;
    class GameObjectHandle;
    
    struct ComponentType {
        std::string name;
        std::function<TypeInfo(const GameObject*)> getTypeInfo;
    };
    
    using ComponentTypeCollection = std::vector<ComponentType>;
    
    class GameWorld {
    private:
        
        struct ComponentInfo {
            ComponentInfo() : getTypeInfo(0) {}
            std::unique_ptr<IContainer> container;
            std::string name;
            std::function<TypeInfo(const GameObject*)> getTypeInfo;
            std::function<IFieldEditor*(const GameObject*)> getFieldEditor;
            std::vector<int> systemsUsingComponent;
        };
    
        using Components = std::vector<ComponentInfo>;
        Components components;
        
        struct SystemInfo {
            SystemInfo() : createFunction(0), deleteFunction(0) {}
            Bitset bitset;
            std::function<IGameSystem*(GameObject*)> createFunction;
            std::function<void(IGameSystem*)> deleteFunction;
            std::string name;
        };
        
        using Systems = std::vector<SystemInfo>;
        Systems systems;
        
        Container<GameObject> objects;
        int componentTypesCount;
        
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
        
        using ComponentTypeFunction = std::function<void(ComponentInfo&)>;
        using SystemTypeFunction = std::function<void(SystemInfo&, std::vector<ComponentId>&)>;
    
        void AddComponentType(ComponentId componentId, const ComponentTypeFunction& function);
        void AddSystemType(SystemId systemId, const SystemTypeFunction& function);
        void RemoveSystemType(SystemId systemId);

        void DoActions(Actions &actions);
        void RemoveRoot(GameObject* root);
        GameObject* CreateEmptyObject(GameObject* parent, GameScene* scene, bool assignId);
        GameObject* CreateObjectFromJson(GameObject* parent, std::istream& jsonStream, const std::function<void(GameObject*)>& objectCreated);
        GameObject* LoadObject(GameObject::AddReferenceComponentList& addReferenceComponents, GameObject* parent, minijson::istream_context &context, const std::function<void(GameObject*)>& objectCreated);
        GameScene* TryGetScene(const std::string& guid);
        GameObject* FindObject(const std::string& guid, int objectId);
        std::string TryFindScenePath(const std::string& guid);
        
        template<typename T>
        void AddSystemType() {
            AddSystemType(GameIdHelper::GetSystemID<T>(), [this] (SystemInfo& systemInfo, std::vector<ComponentId>& components) {
                T::ExtractAllComponents(*this, components);
                systemInfo.createFunction = [] (GameObject* root) {
                    T* system = new T();
                    GameObject** systemRoot = ((GameObject**)&system->root);
                    *(systemRoot) = root;
                    system->index = GameIdHelper::GetSystemID<T>();
                    return system;
                };
                systemInfo.deleteFunction = [] (IGameSystem* system) {
                    delete ((T*)system);
                };
                systemInfo.name = GameIdHelper::GetClassName<T>();
            });
        }
        
        void AddActiveSystem(IGameSystem* system, GameScene* scene);
        void RemoveActiveSystem(IGameSystem* system);
        void SortActiveSystems();
        
       
        
    public:
    
        GameWorld();
        ~GameWorld();
    
        template<typename T>
        void AddComponentType() {
            AddComponentType(GameIdHelper::GetComponentID<T>(), [] (ComponentInfo& componentInfo) {
                componentInfo.container = std::make_unique<Container<T>>();
                componentInfo.name = GameIdHelper::GetClassName<T>();
                T* ptr = 0;
                Meta::static_if<Meta::HasGetTypeFunction::apply<T>::value, T*>(ptr, [&componentInfo](auto p) {
                    using SerializedComponentType = typename std::remove_pointer<decltype(p)>::type;
                    componentInfo.getTypeInfo = [](const GameObject* object) -> TypeInfo {
                        auto component = object->GetComponent<SerializedComponentType>();
                        return component->GetType();
                    };
                    componentInfo.getFieldEditor = [](const GameObject* object) -> IFieldEditor* {
                        auto component = object->GetComponent<SerializedComponentType>();
                        IFieldEditor* editor = FieldEditorCreator<SerializedComponentType>::Create(component);
                        return editor;
                    };
                });
            });
        }
    
        GameObject* CreateRoot();
        GameObject* CreateRootFromJson(std::istream& jsonStream,
                                       const std::function<void(GameObject*)>& rootCreated,
                                       const std::function<void(GameObject*)>& childCreated = 0);
        GameObject* TryFindRoot(const std::string& guid);
        
        const ObjectCollection& Roots();
        
        void Update(float dt);
        void UpdateRoot(float dt, GameObject* root);
        void UpdateActions();
        void Render();
        
        void DebugSystems();
        
        void Clear();
        
        int ObjectCount();
        
        InputManager& Input();
        
        bool TryGetComponentIndex(const std::string& componentName, int& index);
        bool TryGetComponentIndex(const std::string& componentName, int& index, bool& isReference);
        
        ComponentTypeCollection GetComponentTypes() const;
        
        std::function<GameObject*(const std::string& guid)> GuidToRoot;
        std::function<std::string(const std::string& guid)> GuidToPath;
        std::function<void(std::vector<std::string>& guids, std::vector<std::string>& paths)> GetPaths;
        
        static std::string ReadGuidFromJson(std::istream& jsonStream);
        void TryParseJson(std::istream &jsonStream, int componentId,
                          const std::function<void (int, int)>& callback,
                          const std::function<bool (const std::string& componentName)>& componentCallback = 0);
        
        void InvokeChangeToHandles(GameObject* object);
        
        Container<GameScene>& Scenes();
        
        template<typename T>
        void AddComponentTypeWithGetType(const std::function<TypeInfo(const GameObject*)>& getTypeFunction) {
            AddComponentType(GameIdHelper::GetComponentID<T>(), [=] (ComponentInfo& componentInfo) {
                componentInfo.container = std::make_unique<Container<T>>();
                componentInfo.name = GameIdHelper::GetClassName<T>();
                componentInfo.getTypeInfo = getTypeFunction;
            });
        }
        
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
    T* GameObject::CreateSystem() {
        scene->world->AddSystemType<T>();
        return static_cast<T*>(scene->CreateSystem(GameIdHelper::GetSystemID<T>()));
    }
    
    template<typename T>
    void GameObject::RemoveSystem() {
        scene->RemoveSystem(GameIdHelper::GetSystemID<T>());
    }
    
    template<typename T>
    T* GameObject::AddComponent() {
        scene->world->AddComponentType<T>();
        const ComponentId componentId = GameIdHelper::GetComponentID<T>();
        AddComponent(componentId);
        return static_cast<T*>(GetComponent(componentId));
    }

    template<typename T>
    T* GameObject::AddComponent(GameObject* source) {
        scene->world->AddComponentType<T>();
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
        scene->world->AddComponentType<T>();
        const ComponentId componentId = GameIdHelper::GetComponentID<T>();
        ReplaceComponent(componentId, source);
        return static_cast<T*>(source->GetComponent(componentId));
    }
    
    template<typename T>
    Handle<T> GameObject::GetComponentHandle() const {
        const ComponentId componentId = GameIdHelper::GetComponentID<T>();
        if (componentId>=activeComponents.Size()) {
            return Handle<T>();
        }
        if (!activeComponents[componentId]) return Handle<T>();
        const Container<T>* container = static_cast<Container<T>*>(scene->world->components[componentId].container.get());
        return container->GetHandle(componentIndicies[componentId]);
    }

}
