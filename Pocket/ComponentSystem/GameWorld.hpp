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

namespace Pocket {

class GameWorld {
private:
    friend class GameObject;
    friend class IGameSystem;
#if SCRIPTING_ENABLED
    friend class ScriptWorld;
#endif

    using Objects = Container<GameObject>;

    Objects objects;
    
    GameConstants::Systems systems;
    GameConstants::Systems systemsIndexed;
    GameConstants::SystemBitsets systemBitsets;
    
    GameConstants::Components components;
    GameConstants::ComponentNames componentNames;
    GameConstants::ComponentSystems componentSystems;
    
    GameConstants::Actions createActions;
    GameConstants::Actions removeActions;
    
    std::vector<std::function<void*(GameObject*)>> addComponent;
    std::vector<std::function<void*(GameObject*, GameObject*)>> addComponentReference;
    std::vector<std::function<void(GameObject*)>> removeComponent;
    std::vector<std::function<TypeInfo(GameObject*)>> getTypeComponent;
    std::vector<std::function<GameObject*(GameObject*)>> getOwner;
    std::vector<std::function<void*(GameObject*, GameObject*)>> cloneComponent;
    
    struct ObjectID {
        GameObject* object;
        std::string id;
    };
    
    std::vector<ObjectID> objectIDs;

    std::string* GetObjectID(GameObject* object);
    void AddObjectID(GameObject* object, std::string id);
    std::string* FindIDFromReferenceObject(GameObject* referenceObject, int componentID);
    GameObject* FindObjectFromID(const std::string &id);
    GameObject* FindFirstObjectWithComponentID(int componentID);
    
    bool TryGetComponentIndex(std::string componentName, int& index);
    bool TryGetComponentIndex(std::string componentName, int& index, bool& isReference);
    
    GameObject* LoadObject(minijson::istream_context &context, std::function<void(GameObject*)>& onCreated);
    
    void TryAddSystem(int systemID, std::function<IGameSystem*()> createSystem);
    
public:

    template<typename System>
    System* CreateSystem() {
        //assert(ObjectCount() == 0);
        int systemID = IDHelper::GetSystemID<System>();
        if (ObjectCount()==0) {
            TryAddSystem(systemID, []() { return new System(); });
        }
        return (System*)systemsIndexed[systemID];
    }
    
    GameObject* CreateObject();
    GameObject* CreateObject(std::istream &jsonStream, std::function<void(GameObject*)> onCreated = 0);
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
    assert(IDHelper::GetComponentID<Component>()<world->components.size());
    removedComponents[IDHelper::GetComponentID<Component>()] = true;
    
    world->removeActions.emplace_back([this]() {
        auto activeComponentsBefore = activeComponents;
        
        int componentID = IDHelper::GetComponentID<Component>();
        
        auto& systemsUsingComponent = world->componentSystems[componentID];
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
        CheckForScriptSystemsRemoval(world->staticScriptSystemComponents[componentID], activeComponentsBefore, activeScriptComponentsBefore);
#endif
        
        activeComponents[componentID] = false;
        removedComponents[componentID] = false;
        ownedComponents[componentID] = false;
        typename Container<Component>::ObjectInstance* instance = (typename Container<Component>::ObjectInstance*)components[componentID];
        --instance->references;
        if (instance->references == 0) {
            if (instance->owner) {
                GameObject* owner = (GameObject*)instance->owner;
                Container<Component>* container = (Container<Component>*)owner->world->components[componentID];
                container->RemoveObject(instance);
            } else {
                delete instance;
            }
        } else if (instance->owner == this) {
            GameObject* owner = (GameObject*)instance->owner;
            Container<Component>* container = (Container<Component>*)owner->world->components[componentID];
            container->DestroyObject(instance);
        }
        components[componentID] = 0;
    });
}

template<typename Component>
Component* GameObject::AddComponent() {
    if (HasComponent<Component>()) {
        return GetComponent<Component>();
    }
    assert(IDHelper::GetComponentID<Component>()<world->components.size());
    Container<Component>* container = (Container<Component>*)world->components[IDHelper::GetComponentID<Component>()];
    typename Container<Component>::ObjectInstance* instance = container->CreateObject();
    instance->owner = this;
    SetComponent<Component>(instance);
    ownedComponents[IDHelper::GetComponentID<Component>()] = true;
    return GetComponent<Component>();
}

template<typename Component>
Component* GameObject::AddComponent(GameObject* source) {
    if (HasComponent<Component>()) {
        return GetComponent<Component>();
    }
    assert(source);
    assert(!HasComponent<Component>());
    assert(source->HasComponent<Component>());
    assert(IDHelper::GetComponentID<Component>()<world->components.size());
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

template<typename Component>
GameObject* GameObject::GetOwner() {
    typename Container<Component>::ObjectInstance* instance = (typename Container<Component>::ObjectInstance*)components[IDHelper::GetComponentID<Component>()];
    if (!instance) return 0;
    return (GameObject*)instance->owner;
}

template<typename Component>
Component* GameObject::CloneComponent(GameObject* source) {
    assert(source);
    assert(source->HasComponent<Component>());
    typename Container<Component>::ObjectInstance* sourceInstance = (typename Container<Component>::ObjectInstance*)source->components[IDHelper::GetComponentID<Component>()];
    bool isOwnedComponent = source->GetOwner<Component>() == source;
    if (!HasComponent<Component>()) {
        if (isOwnedComponent) {
            AddComponent<Component>();
        } else {
            AddComponent<Component>(source);
        }
    }
    typename Container<Component>::ObjectInstance* instance = (typename Container<Component>::ObjectInstance*)components[IDHelper::GetComponentID<Component>()];
    if (isOwnedComponent) {
        instance->object = sourceInstance->object;
    }
    return (Component*)instance;
}

template<typename... ComponentList>
void GameSystem<ComponentList...>::CreateComponents(GameWorld *world, int systemIndex) {
    std::tuple<ComponentList*...> componentsTuple;
    Meta::for_each_in_tuple_non_const(componentsTuple, [world, systemIndex](auto c) {
        auto& components = world->components;
        auto& componentNames = world->componentNames;
        auto& componentSystems = world->componentSystems;
        auto& addComponent = world->addComponent;
        auto& addComponentReference = world->addComponentReference;
        auto& removeComponent = world->removeComponent;
        auto& cloneComponent = world->cloneComponent;
        auto& getTypeComponent = world->getTypeComponent;
        auto& getOwner = world->getOwner;
        
        using ComponentType = std::remove_pointer_t<decltype(c)>;
        int componentID = IDHelper::GetComponentID<ComponentType>();
        if (componentID>=components.size()) {
            components.resize(componentID + 1, 0);
            componentNames.resize(componentID + 1);
            componentSystems.resize(componentID + 1);
            addComponent.resize(componentID + 1);
            addComponentReference.resize(componentID + 1);
            removeComponent.resize(componentID + 1);
            cloneComponent.resize(componentID + 1);
            getTypeComponent.resize(componentID + 1);
            getOwner.resize(componentID + 1);
        }
        if (!components[componentID]) {
            components[componentID] = new Container<ComponentType>;
            components[componentID]->Initialize();
            componentNames[componentID] = IDHelper::GetClassName<ComponentType>();
            addComponent[componentID] = [](GameObject* object) -> void* {
                return object->AddComponent<ComponentType>();
            };
            addComponentReference[componentID] = [](GameObject* object, GameObject* ref) -> void* {
                return object->AddComponent<ComponentType>(ref);
            };
            removeComponent[componentID] = [](GameObject* object) {
                object->RemoveComponent<ComponentType>();
            };
            cloneComponent[componentID] = [](GameObject* object, GameObject* ref) -> void* {
                return object->CloneComponent<ComponentType>(ref);
            };
            
            ComponentType* ptr = 0;
            Meta::static_if<Meta::HasGetTypeFunction::apply<ComponentType>::value, ComponentType*>(ptr, [&getTypeComponent, componentID](auto p) {
                using SerializedComponentType = std::remove_pointer_t<decltype(p)>;
                getTypeComponent[componentID] = [](GameObject* object) -> TypeInfo {
                    auto component = object->GetComponent<SerializedComponentType>();
                    return component->GetType();
                };
            });
            getOwner[componentID] = [](GameObject* object) -> GameObject* {
                return object->GetOwner<ComponentType>();
            };
        }
        world->systemBitsets[systemIndex][componentID] = true;
        componentSystems[componentID].push_back(world->systems[systemIndex]);
    });
}

}