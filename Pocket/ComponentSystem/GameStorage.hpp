//
//  GameStorage.hpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 29/12/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "Container.hpp"
#include "TypeInfo.hpp"
#include "IGameSystem.hpp"
#include "GameWorld.hpp"
#include "GameObjectSerializer.hpp"

namespace Pocket {

    class GameObjectJsonSerializer;
    class ScriptWorld;

    struct ComponentType {
        std::string name;
        std::function<TypeInfo(const GameObject*)> getTypeInfo;
    };
    
    using ComponentTypeCollection = std::vector<ComponentType>;

    class GameStorage {
    private:
        struct ComponentInfo {
            ComponentInfo() : getTypeInfo(nullptr) {}
            std::unique_ptr<IContainer> container;
            std::string name;
            std::function<TypeInfo(const GameObject*)> getTypeInfo;
            std::function<IFieldEditor*(const GameObject*)> getFieldEditor;
            std::vector<int> systemsUsingComponent;
        };
        
        struct SystemInfo {
            SystemInfo() : createFunction(nullptr), deleteFunction(nullptr) {}
            Bitset bitset;
            std::function<IGameSystem*(GameObject*)> createFunction;
            std::function<void(IGameSystem*)> deleteFunction;
            std::string name;
        };
        
        using Components = std::vector<ComponentInfo>;
        Components components;
        
        using Systems = std::vector<SystemInfo>;
        Systems systems;
        
        Container<GameObject> objects;
        int componentTypesCount;
        
        Container<GameScene> scenes;
        
        GameWorld* prefabWorld;
        
        std::unique_ptr<GameObjectSerializer> serializer;
        
        int systemIdCounter;
        int componentIdCounter;
        
        using ComponentTypeFunction = std::function<void(ComponentInfo&)>;
        using SystemTypeFunction = std::function<void(SystemInfo&, std::vector<ComponentId>&)>;
    
        void AddComponentType(ComponentId componentId, const ComponentTypeFunction& function);
        void AddSystemType(SystemId systemId, const SystemTypeFunction& function, const std::function<void()>& finished);
        void RemoveSystemType(SystemId systemId);
        
    public:
        
        bool TryGetComponentIndex(const std::string& componentName, int& index) const;
        bool TryGetComponentIndex(const std::string& componentName, int& index, bool& isReference) const;
        void TryParseComponent(std::istream &stream, int componentId,
                                            const std::function<void (int, int)>& callback,
                                            const std::function<bool (const std::string& componentName)>& componentCallback = nullptr) const;
        GameStorage();
        ~GameStorage();
    
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
            }, [this] () {
                T::CreateSubSystems(*this);
            });
        }
        
        template<typename T>
        void AddComponentTypeWithGetType(const std::function<TypeInfo(const GameObject*)>& getTypeFunction) {
            AddComponentType(GameIdHelper::GetComponentID<T>(), [=] (ComponentInfo& componentInfo) {
                componentInfo.container = std::make_unique<Container<T>>();
                componentInfo.name = GameIdHelper::GetClassName<T>();
                componentInfo.getTypeInfo = getTypeFunction;
            });
        }
        
        ComponentTypeCollection GetComponentTypes() const;
        
        void SerializeAndRemoveComponents(std::ostream& stream, const SerializePredicate &predicate);
        void DeserializeAndAddComponents(std::istream &jsonStream);
        
        template<typename T>
        void CreateSerializer() {
            serializer = std::make_unique<T>();
        }
        
        GameObject* LoadPrefab(const std::string& guid, std::istream& stream);
        GameObject* TryGetPrefab(const std::string& guid, int objectId = 1);
        void ApplyPrefab(GameObject* prefab, GameObject* scene);
        
        std::function<GameObject*(const std::string& guid)> GuidToRoot;
        std::function<std::string(const std::string& guid)> GuidToPath;
        std::function<void(std::vector<std::string>& guids, std::vector<std::string>& paths)> GetPaths;
        std::function<void(GameObject* prefab)> PrefabLoaded;
        
        friend class GameObjectHandle;
        friend class GameWorld;
        friend class GameObject;
        friend class GameScene;
        friend class GameObjectJsonSerializer;
        friend class ScriptWorld;
    };
}
