//
//  GameWorld.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/12/15.
//  Copyright © 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "IGameWorld.hpp"
#include "GameSettings.hpp"
#include "Container.hpp"
#include "minijson_writer.hpp"
#include "minijson_reader.hpp"
#include <type_traits>
#include <assert.h>
#ifdef SCRIPTING_ENABLED
#include "IScriptSystem.hpp"
#endif
#include "GameObjectSpecific.hpp"
#include "IDHelper.hpp"

template<typename Settings>
class GameWorld : public IGameWorld {
private:

    friend class GameObjectSpecific<Settings>;
    
    using GameObjectSpecific = GameObjectSpecific<Settings>;
    
    using Objects = Container<GameObjectSpecific>;
    
    using Systems = typename Settings::SystemsTuple;
    using InitializeSystems = typename Settings::InitializeSystems;
    using UpdateSystems = typename Settings::UpdateSystems;
    using RenderSystems = typename Settings::RenderSystems;
    using ObjectAddedSystems = typename Settings::ObjectAddedSystems;
    using ObjectRemovedSystems = typename Settings::ObjectRemovedSystems;
    
    using ComponentSystems = typename Settings::ComponentSystemsTuple;
    
    using SystemBitsets = typename Settings::SystemBitsets;
    using Components = typename Settings::AllComponents;
    using SerializableComponents = typename Settings::SerializableComponents;
    using ComponentNames = typename Settings::ComponentNames;
    
    using Actions = std::vector<std::function<void()>>;
    
    Objects objects;
    
    Systems systems;
    
    InitializeSystems initializeSystems;
    UpdateSystems updateSystems;
    RenderSystems renderSystems;
    ObjectAddedSystems objectAddedSystems;
    ObjectRemovedSystems objectRemovedSystems;
    
    ComponentSystems componentSystems;
    
    SystemBitsets systemBitsets;
    Components components;
    
    SerializableComponents serializableComponents;
    ComponentNames componentNames;
    
    Actions createActions;
    Actions removeActions;
    
    
    void InitializeSystemBitsets() {
        meta::for_each_in_tuple(systems, [&](auto& system) {
            using SystemType = std::remove_const_t<std::remove_reference_t<decltype(system)>>;
            meta::for_each_in_tuple(meta::IteratorPointer<typename SystemType::Components>{}.Iterate(), [&,this](auto component) {
                systemBitsets[Settings::template GetSystemID<SystemType>()]
                             [Settings::template GetComponentID<std::remove_pointer_t< decltype(component) >>()] = true;
            });
        });
    }
    
    void InitializeComponentNames() {
        componentNames = Settings::GetComponentNames();
    }
    
    void InitializeCommands() {
        meta::for_each_in_tuple(components, [this] (auto& component) {
            using ComponentType = meta::mp_rename<std::remove_const_t<std::remove_reference_t<decltype(component)>>, meta::ReturnContainedType>;
            int componentIndex = IDHelper::GetComponentID<ComponentType>();
            if (componentIndex>=commands.size()) {
                commands.resize(componentIndex+1);
            }
            commands[componentIndex].addComponent = [](GameObject* gameObjectPtr) {
              GameObjectSpecific* go = (GameObjectSpecific*)gameObjectPtr;
              return go->template AddComponent<ComponentType>();
            };
            commands[componentIndex].addComponentReference = [](GameObject* gameObjectPtr, GameObject* source) {
              GameObjectSpecific* go = (GameObjectSpecific*)gameObjectPtr;
              return go->template AddComponent<ComponentType>((GameObjectSpecific*) source);
            };
            commands[componentIndex].removeComponent = [](void* gameObjectPtr) -> void* {
              GameObjectSpecific* go = (GameObjectSpecific*)gameObjectPtr;
              go->template RemoveComponent<ComponentType>();
              return 0;
            };
        });
        
        meta::for_each_in_tuple(systems, [&](auto& system) {
            using SystemType = std::remove_const_t<std::remove_reference_t<decltype(system)>>;
            
            int systemIndex = IDHelper::GetSystemID<SystemType>();
            if (systemIndex>=getSystemCommands.size()) {
                getSystemCommands.resize(systemIndex+1);
            }
            getSystemCommands[systemIndex] = [this]() -> void* {
                return &GetSystem<SystemType>();
            };
        });
    }
    
    GameObjectSpecific* LoadObject(minijson::istream_context &context, std::function<void(GameObject*)>& onCreated) {
        GameObjectSpecific* object = 0;
         minijson::parse_object(context, [&] (const char* n, minijson::value v) {
            std::string name = n;
            if (name == "GameObject" && v.type() == minijson::Object) {
                object = (GameObjectSpecific*)CreateObject();
                minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                    std::string name = n;
                    if (name == "Components" && v.type() == minijson::Array && object) {
                        minijson::parse_array(context, [&] (minijson::value v) {
                            if (v.type() == minijson::Object) {
                                minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                                    object->AddComponent(context, n);
                                });
                            }
                        });
                    } else if (name == "Children" && v.type() == minijson::Array && object) {
                        minijson::parse_array(context, [&] (minijson::value v) {
                            GameObjectSpecific* child = LoadObject(context, onCreated);
                            if (child) {
                                child->Parent = object;
                            }
                        });
                    }
                    
                    if (onCreated) {
                        onCreated(object);
                    }
                });
            }
        });
        return object;
    }
    
public:

    template<typename System>
    System& GetSystem() {
        return std::get<Settings::template GetSystemID<System>()>(systems);
    }

    GameObject* CreateObject() override {
        auto object = objects.CreateObjectNoReset();
        object->object.instance = object;
        object->object.SetWorld(this);
        object->object.Reset();
        return &object->object;
    }
    
    GameObject* CreateObject(std::istream &jsonStream, std::function<void(GameObject*)> onCreated) override {
        minijson::istream_context context(jsonStream);
        GameObjectSpecific* object = 0;
        try {
            object = LoadObject(context, onCreated);
        } catch (std::exception e) {
            std::cout << e.what() << std::endl;
        }
        return object;
    }
    
    GameWorld() {
        InitializeSystemBitsets();
        InitializeComponentNames();
        InitializeCommands();
        meta::for_each_in_tuple_non_const(components, [](auto& container) {
            container.Initialize();
        });
    }

    void Initialize() {
        meta::for_each_in_tuple(initializeSystems, [&](auto system) {
            std::get<
                Settings::template GetSystemID<
                    std::remove_pointer_t<decltype(system)>
                >()
            >(systems).Initialize(this);
        });
    }

    void Update(float dt) {
        DoActions(createActions);
        DoActions(removeActions);
        meta::for_each_in_tuple(updateSystems, [this, dt] (auto system) {
            std::get<
                    Settings::template GetSystemID<
                        std::remove_pointer_t< decltype(system)>
                    >()
                >(systems).Update(dt);
        });
        
#ifdef SCRIPTING_ENABLED
        for(auto scriptSystem : scriptSystems) {
            scriptSystem->Update(dt);
        }
#endif
    }
    
    void Render() {
        meta::for_each_in_tuple(renderSystems, [this] (auto system) {
            std::get<
                    Settings::template GetSystemID<
                        std::remove_pointer_t< decltype(system)>
                    >()
                >(systems).Render();
        });
    }

    void DoActions(Actions& list) {
       for(int i=0; i<list.size(); ++i) {
            list[i]();
       }
       list.clear();
    }

    int ObjectCount() override {
        return objects.Size();
    }
    
    GameObject* GetObject(int index) override {
        return objects.Get(index);
    }

    void Clear() {
        for(int i=0; i<objects.Size(); ++i) {
            objects.Get(i)->Remove();
        }
        DoActions(createActions);
        DoActions(removeActions);
    }
    
    ~GameWorld() {
        Clear();
    }
    
    void EnumerateComponentClasses(std::function<void(std::string, int)> callback) {
        meta::for_each_in_tuple(serializableComponents, [this, callback] (auto componentPointer) {
            using ComponentType = std::remove_const_t< std::remove_pointer_t<decltype(componentPointer)> >;
            callback(componentNames[Settings::template GetComponentID<ComponentType>()], Settings::template GetComponentID<ComponentType>());
        });
    }
    
#ifdef SCRIPTING_ENABLED
    //Scripting
    using StaticScriptSystemComponents = std::array<std::vector<short>, typename Settings::UniqueComponents{}.size()>;
    StaticScriptSystemComponents staticScriptSystemComponents;
    using ScriptSystemComponents = std::vector<std::vector<short>>;
    ScriptSystemComponents dynamicScriptSystemComponents;
    
    using ScriptComponents = std::vector<Container<void*>>;
    ScriptComponents scriptComponents;
    
    using ScriptBitset = std::vector<bool>;
    
    struct ScriptSystemData {
        typename Settings::Bitset staticComponents;
        std::vector<short> scriptComponents;
    };
    
    using ScriptSystems = std::vector<IScriptSystem*>;
    ScriptSystems scriptSystems;
    
    using ScriptSystemsData = std::vector<ScriptSystemData>;
    ScriptSystemsData scriptSystemsData;
    
    void ClearScripingData(std::function<void(IScriptSystem*)> onSystemDeleted) {
        for(int i=0; i<ObjectCount(); ++i) {
            GetObject(i)->ClearScriptingData();
        }

        for(int i=0; i<typename Settings::UniqueComponents{}.size(); ++i) {
            staticScriptSystemComponents[i].clear();
        }
        dynamicScriptSystemComponents.clear();
        scriptComponents.clear();
        for(auto scriptSystem : scriptSystems) {
            onSystemDeleted(scriptSystem);
        }
        scriptSystems.clear();
        scriptSystemsData.clear();
    }

    void InitializeScriptData(int numSystems, int numComponents,
                std::function<IScriptSystem*(int)> onSystemCreate,
                std::function<void(Container<void*>&, int)> onComponentCreate,
                std::function<void(
                            StaticScriptSystemComponents& staticScriptSystemComponents,
                            ScriptSystemComponents& dynamicScriptSystemComponents,
                            ScriptSystemsData& scriptSystemsData)> onSystemData
                
                ) {
        
        for(int i=0; i<numSystems; i++) {
            scriptSystems.push_back(onSystemCreate(i));
        }
        
        scriptComponents.resize(numComponents);
        for(int i=0; i<numComponents; ++i) {
            onComponentCreate(scriptComponents[i], i);
        }
        
        dynamicScriptSystemComponents.resize(numComponents);
        onSystemData(staticScriptSystemComponents, dynamicScriptSystemComponents, scriptSystemsData);
        
        for(int i=0; i<ObjectCount(); ++i) {
            GetObject(i)->InitializeScriptingData();
        }
    }
#endif
};