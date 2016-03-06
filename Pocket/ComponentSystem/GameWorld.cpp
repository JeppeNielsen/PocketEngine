//
//  GameWorld.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 03/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameWorld.hpp"


bool GameWorld::TryGetComponentIndex(std::string componentName, int& index) {
    for(int i=0; i<componentNames.size(); ++i) {
        if (componentNames[i]==componentName) {
            index = i;
            return true;
        }
    }
    return false;
}

GameObject* GameWorld::LoadObject(minijson::istream_context &context, std::function<void(GameObject*)>& onCreated) {
    GameObject* object = 0;
     minijson::parse_object(context, [&] (const char* n, minijson::value v) {
        std::string name = n;
        if (name == "GameObject" && v.type() == minijson::Object) {
            object = (GameObject*)CreateObject();
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
                        GameObject* child = LoadObject(context, onCreated);
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

void GameWorld::InitializeWorld() {
    std::vector<IGameSystem*> initializedSystems;
    while (true) {
        std::vector<IGameSystem*> systemsToInitialize;
        for (auto s : systems) {
            if (std::find(initializedSystems.begin(), initializedSystems.end(), s)==initializedSystems.end()) {
                systemsToInitialize.push_back(s);
            }
        }
        if (systemsToInitialize.empty()) break;
        for(auto s : systemsToInitialize) {
            initializedSystems.push_back(s);
            s->Initialize(this);
        }
    }
    
    systemBitsets.resize(systems.size());
    componentSystems.resize(systems.size());
    for(int i=0; i<systems.size(); ++i) {
        systems[i]->CreateComponents(this, i);
    }
    for(auto c : components) {
        c->Initialize();
    }
#if SCRIPTING_ENABLED
    staticScriptSystemComponents.resize(components.size());
#endif
}

GameObject* GameWorld::CreateObject() {
    auto object = objects.CreateObjectNoReset();
    object->object.instance = object;
    object->object.SetWorld(this);
    object->object.Reset();
    return &object->object;
}

GameObject* GameWorld::CreateObject(std::istream &jsonStream, std::function<void(GameObject*)> onCreated) {
    minijson::istream_context context(jsonStream);
    GameObject* object = 0;
    try {
        object = LoadObject(context, onCreated);
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
    }
    return object;
}

GameWorld::GameWorld() {
    objects.Initialize();
}

void GameWorld::Update(float dt) {
    DoActions(createActions);
    DoActions(removeActions);
    
    for(auto system : systems) {
        system->Update(dt);
    }
    
#ifdef SCRIPTING_ENABLED
    for(auto scriptSystem : scriptSystems) {
        scriptSystem->Update(dt);
    }
#endif
}

void GameWorld::Render() {
    for(auto system : systems) {
        system->Render();
    }
}

void GameWorld::DoActions(GameConstants::Actions& list) {
   for(int i=0; i<list.size(); ++i) {
        list[i]();
   }
   list.clear();
}

int GameWorld::ObjectCount() {
    return objects.Size();
}

GameObject* GameWorld::GetObject(int index) {
    return objects.Get(index);
}

void GameWorld::Clear() {
    for(int i=0; i<objects.Size(); ++i) {
        objects.Get(i)->Remove();
    }
    DoActions(createActions);
    DoActions(removeActions);
}

GameWorld::~GameWorld() {
    Clear();
    for(auto c : components) {
        delete c;
    }
    for(auto s : systems) {
        delete s;
    }
}


#ifdef SCRIPTING_ENABLED
void GameWorld::ClearScripingData(std::function<void(IScriptSystem*)> onSystemDeleted) {
    for(int i=0; i<ObjectCount(); ++i) {
        GetObject(i)->ClearScriptingData();
    }

    for(int i=0; i<components.size(); ++i) {
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

void GameWorld::InitializeScriptData(int numSystems, int numComponents,
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










