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
#include "GameObject.hpp"

namespace Pocket {

    class GameWorld;
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
        
        using ComponentTypeFunction = std::function<void(ComponentInfo&)>;
        using SystemTypeFunction = std::function<void(SystemInfo&, std::vector<ComponentId>&)>;
    
        void AddComponentType(ComponentId componentId, const ComponentTypeFunction& function);
        void AddSystemType(SystemId systemId, const SystemTypeFunction& function);
        void RemoveSystemType(SystemId systemId);
        bool TryGetComponentIndex(const std::string& componentName, int& index);
        bool TryGetComponentIndex(const std::string& componentName, int& index, bool& isReference);
    
    public:
    
        GameStorage();
    
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
        
        friend class GameObjectHandle;
        friend class GameWorld;
        friend class GameObject;
        friend class GameScene;
        friend class GameObjectJsonSerializer;
        friend class ScriptWorld;
    };
}
