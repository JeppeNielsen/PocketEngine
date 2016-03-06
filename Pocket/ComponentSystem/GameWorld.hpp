//
//  GameWorld.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/12/15.
//  Copyright © 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include "GameConstants.hpp"
#include "minijson_writer.hpp"
#include "minijson_reader.hpp"
#include <type_traits>
#include <assert.h>
#include "GameObject.hpp"
#include "IDHelper.hpp"
#include "MetaLibrary.hpp"
#ifdef SCRIPTING_ENABLED
#include "IScriptSystem.hpp"
#endif
#include "GameSystem.hpp"
#include "Container.hpp"

class GameWorld {
private:
    friend class GameObject;
#if SCRIPTING_ENABLED
    friend class ScriptWorld;
#endif

    using Objects = Container<GameObject>;

    Objects objects;
    
    GameConstants::Systems systems;
    GameConstants::SystemBitsets systemBitsets;
    
    GameConstants::Components components;
    GameConstants::ComponentNames componentNames;
    GameConstants::ComponentSystems componentSystems;
    
    GameConstants::Actions createActions;
    GameConstants::Actions removeActions;
    
    std::vector<std::function<void*(GameObject*)>> addComponent;
    std::vector<std::function<void(GameObject*)>> removeComponent;
    std::vector<std::function<TypeInfo(GameObject*)>> getTypeComponent;
    
    bool isInitializing;
    
    bool TryGetComponentIndex(std::string componentName, int& index);
    GameObject* LoadObject(minijson::istream_context &context, std::function<void(GameObject*)>& onCreated);
    void InitializeWorld();
    
    
    
public:

    template<typename... Systems>
    void Initialize() {
        isInitializing = true;
        std::tuple<Systems*...> systemsTuple;
        Meta::for_each_in_tuple_non_const(systemsTuple, [this](auto t) {
            using SystemType = std::remove_pointer_t<decltype(t)>;
            AddSystem<SystemType>();
        });
        InitializeWorld();
        isInitializing = false;
    }

    template<typename System>
    System* GetSystem() {
        if (isInitializing) return AddSystem<System>();
        return (System*)systems[IDHelper::GetSystemID<System>()];
    }
    
    template<typename System>
    System* AddSystem() {
        if (!isInitializing) return GetSystem<System>();
        int systemID = IDHelper::GetSystemID<System>();
        if (systemID>=systems.size()) {
            systems.resize(systemID + 1, 0);
        }
        if (!systems[systemID]) {
            systems[systemID] = new System();
            systems[systemID]->index = (int)(systems.size() - 1);
        }
        return (System*)systems[systemID];
    }
    
    GameObject* CreateObject();
    GameObject* CreateObject(std::istream &jsonStream, std::function<void(GameObject*)> onCreated);
    GameWorld();
    
    void Update(float dt);
    void Render();
    void DoActions(GameConstants::Actions& list);
    int ObjectCount();
    GameObject* GetObject(int index);
    void Clear();
    ~GameWorld();
    
#ifdef SCRIPTING_ENABLED
    //Scripting
    using StaticScriptSystemComponents = std::vector<std::vector<short>>;
    StaticScriptSystemComponents staticScriptSystemComponents;
    using ScriptSystemComponents = std::vector<std::vector<short>>;
    ScriptSystemComponents dynamicScriptSystemComponents;
    
    using ScriptComponents = std::vector<Container<void*>>;
    ScriptComponents scriptComponents;
    
    struct ScriptSystemData {
        GameConstants::Bitset staticComponents;
        std::vector<short> scriptComponents;
    };
    
    using ScriptSystems = std::vector<IScriptSystem*>;
    ScriptSystems scriptSystems;
    
    using ScriptSystemsData = std::vector<ScriptSystemData>;
    ScriptSystemsData scriptSystemsData;
    
    void ClearScripingData(std::function<void(IScriptSystem*)> onSystemDeleted);
    void InitializeScriptData(int numSystems, int numComponents,
                std::function<IScriptSystem*(int)> onSystemCreate,
                std::function<void(Container<void*>&, int)> onComponentCreate,
                std::function<void(
                            StaticScriptSystemComponents& staticScriptSystemComponents,
                            ScriptSystemComponents& dynamicScriptSystemComponents,
                            ScriptSystemsData& scriptSystemsData)> onSystemData
                
                );
#endif
};



template<typename Component>
void GameObject::RemoveComponent() {
    if (removedComponents[IDHelper::GetComponentID<Component>()]) return;
    assert(HasComponent<Component>());
    removedComponents[IDHelper::GetComponentID<Component>()] = true;
    
    world->removeActions.emplace_back([this]() {
        auto activeComponentsBefore = activeComponents;
        
        auto& systemsUsingComponent = world->componentSystems[IDHelper::GetComponentID<Component>()];
        for(auto system : systemsUsingComponent) {
            auto& bitSet = world->systemBitsets[system->index];
            bool wasInterest = (activeComponentsBefore & bitSet) == bitSet;
            if (wasInterest) {
            
                system->ObjectRemoved(this);
                
                int objectIndexInSystem = systemIndices[system->index];
                int lastIndex = (int)system->objects.size() - 1;
                GameObject* lastObject = (GameObject*)system->objects[lastIndex];
                lastObject->systemIndices[system->index]=objectIndexInSystem;
                system->objects[objectIndexInSystem]=lastObject;
                system->objects.pop_back();
            }
        }
        
#ifdef SCRIPTING_ENABLED
        auto activeScriptComponentsBefore = activeScriptComponents;
        CheckForScriptSystemsRemoval(world->staticScriptSystemComponents[IDHelper::GetComponentID<Component>()], activeComponentsBefore, activeScriptComponentsBefore);
#endif
        
        int componentID = IDHelper::GetComponentID<Component>();
        
        activeComponents[componentID] = false;
        removedComponents[componentID] = false;
        typename Container<Component>::ObjectInstance* instance = (typename Container<Component>::ObjectInstance*)components[componentID];
        Container<Component>* container = (Container<Component>*)world->components[componentID];
        container->RemoveObject(instance);
        components[componentID] = 0;
    });
}

template<typename Component>
Component* GameObject::AddComponent() {
    assert(!HasComponent<Component>());
    Container<Component>* container = (Container<Component>*)world->components[IDHelper::GetComponentID<Component>()];
    SetComponent<Component>(container->CreateObject());
    return GetComponent<Component>();
}

template<typename Component>
Component* GameObject::AddComponent(GameObject* source) {
    assert(source);
    assert(!HasComponent<Component>());
    assert(source->HasComponent<Component>());
    typename Container<Component>::ObjectInstance* instance = (typename Container<Component>::ObjectInstance*)source->components[IDHelper::GetComponentID<Component>()];
    ++instance->references;
    SetComponent<Component>(instance);
    return GetComponent<Component>();
}

template<typename Component>
void GameObject::SetComponent(typename Container<Component>::ObjectInstance *instance) {
    activeComponents[IDHelper::GetComponentID<Component>()] = true;
    components[IDHelper::GetComponentID<Component>()] = instance;
    auto activeComponentsBefore = activeComponents;
#ifdef SCRIPTING_ENABLED
    auto activeScriptComponentsBefore = activeScriptComponents;
#endif
    world->createActions.emplace_back([this, activeComponentsBefore
#ifdef SCRIPTING_ENABLED
    , activeScriptComponentsBefore
#endif
    ]() {
    
        auto& systemsUsingComponent = world->componentSystems[IDHelper::GetComponentID<Component>()];
        for(auto system : systemsUsingComponent) {
            auto& bitSet = world->systemBitsets[system->index];
            bool isInterest = (activeComponentsBefore & bitSet) == bitSet;
            if (isInterest) {
                systemIndices[system->index]=(int)system->objects.size();
                system->objects.push_back(this);
                system->ObjectAdded(this);
            }
        }

#ifdef SCRIPTING_ENABLED
        CheckForScriptSystemsAddition(world->staticScriptSystemComponents[IDHelper::GetComponentID<Component>()], activeComponentsBefore, activeScriptComponentsBefore);
#endif
    });
}

template<typename... ComponentList>
void GameSystem<ComponentList...>::CreateComponents(GameWorld *world, int systemIndex) {
    std::tuple<ComponentList*...> componentsTuple;
    Meta::for_each_in_tuple_non_const(componentsTuple, [world, systemIndex](auto c) {
        auto& components = world->components;
        auto& componentNames = world->componentNames;
        auto& componentSystems = world->componentSystems;
        auto& addComponent = world->addComponent;
        auto& removeComponent = world->removeComponent;
        auto& getTypeComponent = world->getTypeComponent;
        
        using ComponentType = std::remove_pointer_t<decltype(c)>;
        int componentID = IDHelper::GetComponentID<ComponentType>();
        if (componentID>=components.size()) {
            components.resize(componentID + 1, 0);
            componentNames.resize(componentID + 1);
            componentSystems.resize(componentID + 1);
            addComponent.resize(componentID + 1);
            removeComponent.resize(componentID + 1);
            getTypeComponent.resize(componentID + 1);
        }
        if (!components[componentID]) {
            components[componentID] = new Container<ComponentType>;
            componentNames[componentID] = IDHelper::GetClassName<ComponentType>();
            addComponent[componentID] = [](GameObject* object) -> void* {
                return object->AddComponent<ComponentType>();
            };
            removeComponent[componentID] = [](GameObject* object) {
                object->RemoveComponent<ComponentType>();
            };
            
            ComponentType* ptr = 0;
            Meta::static_if<Meta::HasGetTypeFunction::apply<ComponentType>::value, ComponentType*>(ptr, [&getTypeComponent, componentID](auto p) {
                using SerializedComponentType = std::remove_pointer_t<decltype(p)>;
                getTypeComponent[componentID] = [](GameObject* object) -> TypeInfo {
                    auto component = object->GetComponent<SerializedComponentType>();
                    return component->GetType();
                };
            });
        }
        world->systemBitsets[systemIndex][componentID] = true;
        componentSystems[componentID].push_back(world->systems[systemIndex]);
    });
}