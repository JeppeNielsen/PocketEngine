//
//  GameWorld.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/12/15.
//  Copyright © 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSettings.hpp"
#include "Container.hpp"
#include "minijson_writer.hpp"
#include "minijson_reader.hpp"
#include <type_traits>
#include <assert.h>
#include "Property.hpp"
#ifdef SCRIPTING_ENABLED
#include "IScriptSystem.hpp"
#endif
#include "GameObject.hpp"

template<typename Settings>
class GameWorld {
private:

    friend class GameObject<Settings>;
    
    using GameObject = GameObject<Settings>;
    
    using Objects = Container<GameObject>;
    
    using Systems = typename Settings::SystemsTuple;
    using InitializeSystems = typename Settings::InitializeSystems;
    using UpdateSystems = typename Settings::UpdateSystems;
    using RenderSystems = typename Settings::RenderSystems;
    
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
            std::get<SystemType>(systems).world = this;
        });
    }
    
    void InitializeComponentNames() {
        componentNames = Settings::GetComponentNames();
    }
public:

    template<typename System>
    System& GetSystem() {
        return std::get<Settings::template GetSystemID<System>()>(systems);
    }

    GameObject* CreateObject() {
        auto object = objects.CreateObjectNoReset();
        object->object.instance = object;
        object->object.SetWorld(this);
        object->object.Reset();
        return &object->object;
    }
    
    GameObject* CreateObject(std::istream &jsonStream, std::function<void(GameObject*)> onCreated) {
        minijson::istream_context context(jsonStream);
        GameObject* object = 0;
        try {
             minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                std::string name = n;
                if (name == "GameObject" && v.type() == minijson::Object) {
                    object = CreateObject();
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
                        } /*else if (name == "Children" && v.type() == minijson::Array && object) {
                            minijson::parse_array(context, [&] (minijson::value v) {
                                GameObject* child = CreateGameObjectJson(context, iterator);
                                if (child) {
                                    child->Parent = object;
                                }
                            });
                        }
                        */
                        if (onCreated) {
                            onCreated(object);
                        }
                    });
                }
             });
        } catch (std::exception e) {
            std::cout << e.what() << std::endl;
        }
        return object;
    }
    
    GameWorld() {
        InitializeSystemBitsets();
        InitializeComponentNames();
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
            >(systems).Initialize();//*this);
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

    int ObjectCount() {
        return objects.Size();
    }
    
    GameObject* GetObject(int index) {
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