//
//  GameObjectSpecific.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 12/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameObject.hpp"

template<typename Settings>
class GameWorld;

#ifdef SCRIPTING_ENABLED

struct IGameObject {
    virtual void* GetComponent(int componentID) = 0;
    virtual void* AddComponent(int componentID) = 0;
    virtual void RemoveComponent(int componentID) = 0;
    virtual void* GetScriptComponent(int componentID) = 0;
    virtual void* AddScriptComponent(int componentID) = 0;
    virtual void RemoveScriptComponent(int componentID) = 0;
};

#endif

template<typename Settings>
class GameObjectSpecific
#ifdef SCRIPTING_ENABLED
: public IGameObject
#endif
: public GameObject
{
private:
    friend class GameWorld<Settings>;
    
    using GameWorld = GameWorld<Settings>;
    
    typename Settings::Bitset activeComponents;
    typename Settings::Bitset removedComponents;
    
    std::array<int, typename Settings::Systems{}.size()> indexInSystem;
    
    bool isRemoved;
    typename Container<GameObjectSpecific>::ObjectInstance* instance;
    GameWorld* world;
    
    friend class Container<GameObjectSpecific>::ObjectInstance;
    
    void Reset() {
        isRemoved = false;
        activeComponents.reset();
        removedComponents.reset();
#ifdef SCRIPTING_ENABLED
        for(int i=0; i<activeScriptComponents.size(); ++i) {
            activeScriptComponents[i]=false;
            removedScriptComponents[i]=false;
        }
#endif
    }
    
    GameObjectSpecific() : world(0) { }
    
    void SetWorld(GameWorld* w) {
        if (world) return;
        world = w;
        commands = &world->commands;
#ifdef SCRIPTING_ENABLED
        InitializeScriptingData();
#endif
    }
    
    GameObjectSpecific(const GameObjectSpecific& other) = default;
    
public:
    
    void Remove() override {
        if (isRemoved) return;
        isRemoved = true;
        
        meta::for_each_in_tuple(world->components, [this] (auto& component) {
            using ComponentType = meta::mp_rename<std::remove_const_t<std::remove_reference_t<decltype(component)>>, meta::ReturnContainedType>;
            if (HasComponent<ComponentType>()) {
                RemoveComponent<ComponentType>();
            }
        });
        
        world->removeActions.emplace_back([this]() {
            world->objects.RemoveObject(instance);
            isRemoved = false;
        });
        
        for(auto child : children) {
            child->Remove();
        }
    }
    
    template<typename Component>
    bool HasComponent() const {
        return activeComponents[Settings::template GetComponentID<Component>()];
    }
    
    template<typename Component>
    Component* GetComponent() {
        if (!HasComponent<Component>()) {
            return 0;
        }
        return &((typename Container<Component>::ObjectInstance*)components[IDHelper::GetComponentID<Component>()])->object;
    }
    
    template<typename Component>
    void RemoveComponent() {
        if (removedComponents[Settings::template GetComponentID<Component>()]) return;
        assert(HasComponent<Component>());
        removedComponents[Settings::template GetComponentID<Component>()] = true;
        
        world->removeActions.emplace_back([this]() {
            auto activeComponentsBefore = activeComponents;
            
            meta::for_each_in_tuple(std::get< Settings::template GetComponentID<Component>() >(world->componentSystems), [this, activeComponentsBefore] (auto systemPointer) {
                using SystemType = std::remove_const_t<std::remove_pointer_t<decltype(systemPointer)>>;
                auto& system = std::get< Settings::template GetSystemID< SystemType >() >(world->systems);
                auto& bitSet = world->systemBitsets[Settings::template GetSystemID<std::remove_reference_t<decltype(system)> >()];
                bool wasInterest = (activeComponentsBefore & bitSet) == bitSet;
                if (wasInterest) {
                
                    
                    
                    meta::static_if<meta::find_index<meta::as_list<decltype(world->objectRemovedSystems)>, decltype(systemPointer)> {} != meta::npos{}>(systemPointer, [this](auto ptr) {
                        using RemovedObjectSystemType = std::remove_const_t<std::remove_pointer_t<decltype(ptr)>>;
                        auto& removedObjectSystem = std::get< Settings::template GetSystemID< RemovedObjectSystemType >() >(world->systems);
                        removedObjectSystem.ObjectRemoved(this);
                    });
                
                    int objectIndexInSystem = indexInSystem[Settings::template GetSystemID< std::remove_reference_t<decltype(system)> >()];
                    int lastIndex = (int)system.objects.size() - 1;
                    GameObjectSpecific* lastObject = (GameObjectSpecific*)system.objects[lastIndex];
                    lastObject->indexInSystem[Settings::template GetSystemID< std::remove_reference_t<decltype(system)> >()]=objectIndexInSystem;
                    system.objects[objectIndexInSystem]=lastObject;
                    system.objects.pop_back();
                }
            });
            
#ifdef SCRIPTING_ENABLED
            auto activeScriptComponentsBefore = activeScriptComponents;
            CheckForScriptSystemsRemoval(world->staticScriptSystemComponents[Settings::template GetComponentID<Component>()], activeComponentsBefore, activeScriptComponentsBefore);
#endif
            
            activeComponents[Settings::template GetComponentID<Component>()] = false;
            removedComponents[Settings::template GetComponentID<Component>()] = false;
            int componentID = IDHelper::GetComponentID<Component>();
            typename Container<Component>::ObjectInstance* instance = (typename Container<Component>::ObjectInstance*)components[componentID];
            auto& container = std::get<Settings::template GetComponentID<Component>()>(world->components);
            container.RemoveObject(instance);
            components[componentID] = 0;
        });
    }
    
    template<typename Component>
    Component* AddComponent() {
        assert(!HasComponent<Component>());
        auto& container = std::get<Settings::template GetComponentID<Component>()>(world->components);
        SetComponent<Component>(container.CreateObject());
        return GetComponent<Component>();
    }
    
    template<typename Component>
    Component* AddComponent(GameObjectSpecific* source) {
        assert(source);
        assert(!HasComponent<Component>());
        assert(source->HasComponent<Component>());
        typename Container<Component>::ObjectInstance* instance = (typename Container<Component>::ObjectInstance*)source->components[Settings::template GetComponentID<Component>()];
        ++instance->references;
        SetComponent<Component>(instance);
        return GetComponent<Component>();
    }
    
    void ToJson(std::ostream& stream, SerializePredicate predicate) override {
        minijson::writer_configuration config;
        config = config.pretty_printing(true);
        minijson::object_writer writer(stream, config);
        WriteJson(writer, predicate);
        writer.close();
    }
    
    std::vector<TypeInfo> GetComponentTypeInfos() {
        std::vector<TypeInfo> infos;
        meta::for_each_in_tuple(world->serializableComponents, [this, &infos] (auto componentPointer) {
            using ComponentType = std::remove_const_t< std::remove_pointer_t<decltype(componentPointer)> >;
            if (HasComponent<ComponentType>()) {
                infos.push_back(GetComponentTypeInfo<ComponentType>());
            }
        });
        return infos;
    }
    
private:
    template<typename Component>
    void SetComponent(typename Container<Component>::ObjectInstance* instance) {
        activeComponents[Settings::template GetComponentID<Component>()] = true;
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
            meta::for_each_in_tuple(std::get< Settings::template GetComponentID<Component>() >(world->componentSystems), [this, activeComponentsBefore] (auto systemPointer) {
                auto& system = std::get< Settings::template GetSystemID< std::remove_pointer_t<decltype(systemPointer)> >() >(world->systems);
                auto& bitSet = world->systemBitsets[Settings::template GetSystemID< std::remove_reference_t<decltype(system)> >()];
                bool isInterest = (activeComponentsBefore & bitSet) == bitSet;
                if (isInterest) {
                    indexInSystem[Settings::template GetSystemID< std::remove_reference_t<decltype(system)> >()]=(int)system.objects.size();
                    system.objects.push_back(this);
                    meta::static_if<meta::find_index<meta::as_list<decltype(world->objectAddedSystems)>, decltype(systemPointer)> {} != meta::npos{}>(systemPointer, [this](auto ptr) {
                        using AddedObjectSystemType = std::remove_const_t<std::remove_pointer_t<decltype(ptr)>>;
                        auto& addedObjectSystem = std::get< Settings::template GetSystemID< AddedObjectSystemType >() >(world->systems);
                        addedObjectSystem.ObjectAdded(this);
                    });
                }
            });
#ifdef SCRIPTING_ENABLED
            CheckForScriptSystemsAddition(world->staticScriptSystemComponents[Settings::template GetComponentID<Component>()], activeComponentsBefore, activeScriptComponentsBefore);
#endif
        });
    }
    
    void WriteJson(minijson::object_writer& writer, SerializePredicate predicate) {

        minijson::object_writer gameObject = writer.nested_object("GameObject");
        minijson::array_writer components = gameObject.nested_array("Components");
        
        meta::for_each_in_tuple(world->serializableComponents, [this, &components, &predicate] (auto componentPointer) {
            using ComponentType = std::remove_const_t< std::remove_pointer_t<decltype(componentPointer)> >;
            if (HasComponent<ComponentType>()) {
                if (!(predicate && !predicate(this, IDHelper::GetComponentID<ComponentType>()))) {
                    SerializeComponent<ComponentType>(components, false, 0);
                }
            }
        });

        components.close();
        
        if (!children.empty()) {
            minijson::array_writer children_object = gameObject.nested_array("Children");
            for(auto child : children) {
                if (predicate && !predicate(child, -1)) {
                    continue;
                }
                GameObjectSpecific* childSpecific = (GameObjectSpecific*)child;
                minijson::object_writer child_object = children_object.nested_object();
                childSpecific->WriteJson(child_object, predicate);
                child_object.close();
            }
            children_object.close();
        }
        
        gameObject.close();
    }
    
    template<typename Component>
    void SerializeComponent(minijson::array_writer& writer, bool isReference, std::string* referenceID ) {
        minijson::object_writer componentWriter = writer.nested_object();
        Component* component = GetComponent<Component>();
        auto type = component->GetType();
        std::string& name = world->componentNames[Settings::template GetComponentID<Component>()];
        if (!isReference) {
            minijson::object_writer jsonComponent = componentWriter.nested_object(name.c_str());
            type.Serialize(jsonComponent);
            jsonComponent.close();
        } else {
            std::string referenceName = name + ":ref";
            minijson::object_writer jsonComponent = componentWriter.nested_object(referenceName.c_str());
            if (!referenceID) {
                jsonComponent.write("id", "");
            } else {
                jsonComponent.write("id", *referenceID);
            }
            jsonComponent.close();
        }
        componentWriter.close();
    }
    
    void AddComponent(minijson::istream_context& context, std::string componentName) {
        bool addedAny = false;
        meta::for_each_in_tuple(world->serializableComponents, [this, &context, &componentName, &addedAny] (auto componentPointer) {
            if (addedAny) {
                return;
            }
            using ComponentType = std::remove_const_t< std::remove_pointer_t<decltype(componentPointer)> >;
            if (componentName == world->componentNames[Settings::template GetComponentID<ComponentType>()]) {
                if (!HasComponent<ComponentType>()) {
                    ComponentType* component = AddComponent<ComponentType>();
                    auto type = component->GetType();
                    type.Deserialize(context);
                    addedAny = true;
                } else {
                    std::cout<<"Only one component per type allowed per object"<<std::endl;
                    minijson::ignore(context);
                }
            }
        });
        if (!addedAny) {
            minijson::ignore(context);
        }
    }
    
    template<typename Component>
    TypeInfo GetComponentTypeInfo() {
        Component* component = GetComponent<Component>();
        return component->GetType();
    }
    
#ifdef SCRIPTING_ENABLED
    //Scripting *******************************
    
    // Scripting Data
    using ScriptComponent = Container<void*>::ObjectInstance*;
    ScriptComponent* scriptComponents;
    
    int* scriptSystemIndices;
    
    typename GameWorld::ScriptBitset activeScriptComponents;
    typename GameWorld::ScriptBitset removedScriptComponents;

    void* GetComponent(int componentID) override {
        //&((typename Container<Component>::ObjectInstance*)components[componentID])->object;
        return components[componentID]; // this works since "object" is first in ObjectInstance;
    }
    private:
    void* AddComponent(int componentID) override {
        int counter = 0;
        
        meta::for_each_in_tuple(world->components, [this, &counter, componentID] (auto& component) {
            if (counter == componentID) {
                using ComponentType = meta::mp_rename<std::remove_const_t<std::remove_reference_t<decltype(component)>>, meta::ReturnContainedType>;
                AddComponent<ComponentType>();
            }
            counter++;
        });
        return GetComponent(componentID);
    }
    
    void RemoveComponent(int componentID) override {
        int counter = 0;
        meta::for_each_in_tuple(world->components, [this, &counter, componentID] (auto& component) {
            if (counter == componentID) {
                using ComponentType = meta::mp_rename<std::remove_const_t<std::remove_reference_t<decltype(component)>>, meta::ReturnContainedType>;
                RemoveComponent<ComponentType>();
            }
            counter++;
        });
    }
    
    

    void ClearScriptingData() {
        delete[] scriptComponents;
        delete[] scriptSystemIndices;
        activeScriptComponents.clear();
        removedScriptComponents.clear();
    }
    
    void InitializeScriptingData() {
        std::size_t numScriptComponents = world->scriptComponents.size();
        scriptComponents = new ScriptComponent[numScriptComponents];
        for (int i=0; i<numScriptComponents; i++) {
            scriptComponents[i]=0;
        }
        scriptSystemIndices = new int[world->scriptSystems.size()];
        activeScriptComponents.resize(numScriptComponents);
        removedScriptComponents.resize(numScriptComponents);
    }
public:
    
    void* GetScriptComponent(int componentID) override {
        if (!activeScriptComponents[componentID]) return 0;
        typename Container<void*>::ObjectInstance* instance = (typename Container<void*>::ObjectInstance*)scriptComponents[componentID];
        return instance->object;
    }

    void* AddScriptComponent(int componentID) override {
        if (activeScriptComponents[componentID]) {
            return scriptComponents[componentID];
        }
        
        activeScriptComponents[componentID] = true;
        scriptComponents[componentID] = world->scriptComponents[componentID].CreateObject();
        
        auto activeComponentsBefore = activeComponents;
        auto activeScriptComponentsBefore = activeScriptComponents;
        world->createActions.emplace_back([this, componentID, activeComponentsBefore, activeScriptComponentsBefore]() {
            CheckForScriptSystemsAddition(world->dynamicScriptSystemComponents[componentID], activeComponentsBefore, activeScriptComponentsBefore);
        });
        
        return scriptComponents[componentID];
    }

    void RemoveScriptComponent(int componentID) override {
        if (!activeScriptComponents[componentID]) return;
        
        if (removedScriptComponents[componentID]) return;
        removedScriptComponents[componentID] = true;
        
        world->removeActions.emplace_back([this, componentID]() {
            
            auto activeScriptComponentsBefore = activeScriptComponents;
            
            activeScriptComponents[componentID] = false;
            removedScriptComponents[componentID] = false;
            
            CheckForScriptSystemsRemoval(world->dynamicScriptSystemComponents[componentID], activeComponents, activeScriptComponentsBefore);
            
            typename Container<void*>::ObjectInstance* instance = (typename Container<void*>::ObjectInstance*)scriptComponents[componentID];
            world->scriptComponents[componentID].RemoveObject(instance);
        });
    }

private:
    void CheckForScriptSystemsAddition(const std::vector<short>& systems, const typename Settings::Bitset& activeComponentsBefore, const typename GameWorld::ScriptBitset& activeScriptComponentsBefore) {
        for(int i = 0; i<systems.size(); ++i) {
            short systemIndex = systems[i];
            IScriptSystem* system = world->scriptSystems[systemIndex];
            auto& systemData = world->scriptSystemsData[systemIndex];
            if (systemData.staticComponents!=0 && !((activeComponentsBefore & systemData.staticComponents) == systemData.staticComponents)) {
                continue;
            }
            std::size_t componentCount = systemData.scriptComponents.size();
            bool match = true;
            for(int j=0; j<componentCount; ++j) {
                if (!activeScriptComponentsBefore[systemData.scriptComponents[j]]) {
                    match = false;
                    break;
                }
            }
            if (!match) continue;
            scriptSystemIndices[systemIndex] = system->AddObject(this);
            system->ObjectAdded(this);
        }
    }
    
    void CheckForScriptSystemsRemoval(const std::vector<short>& systems, const typename Settings::Bitset& activeComponentsBefore, const typename GameWorld::ScriptBitset& activeScriptComponentsBefore) {
    
        for(int i = 0; i<systems.size(); ++i) {
            short systemIndex = systems[i];
            IScriptSystem* system = world->scriptSystems[systemIndex];
            auto& systemData = world->scriptSystemsData[systemIndex];
            if (systemData.staticComponents!=0 && !((activeComponentsBefore & systemData.staticComponents) == systemData.staticComponents)) {
                continue;
            }
            std::size_t componentCount = systemData.scriptComponents.size();
            bool match = true;
            for(int j=0; j<componentCount; ++j) {
                if (!activeScriptComponentsBefore[systemData.scriptComponents[j]]) {
                    match = false;
                    break;
                }
            }
            if (!match) continue;
            system->ObjectRemoved(this);
            int index = scriptSystemIndices[systemIndex];
            auto* last = system->RemoveObject(index);
            GameObjectSpecific* lastGameObject = (GameObjectSpecific*)last;
            lastGameObject->scriptSystemIndices[systemIndex] = index;
        }
    }
#endif
};