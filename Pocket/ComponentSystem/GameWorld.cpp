//
//  GameWorld.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 17/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameWorld.hpp"
#include "StringHelper.hpp"

using namespace Pocket;

GameWorld::GameWorld() : componentTypesCount(0), sortScenes(false) {
    scenes.defaultObject.world = this;
    objects.count = 0;
}
GameWorld::~GameWorld() { Clear(); }

void GameWorld::AddComponentType(ComponentId componentId, const ComponentTypeFunction& function) {
    if (componentId>=components.size()) {
        componentTypesCount = componentId + 1;
        components.resize(componentTypesCount);
        
        if (objects.defaultObject.activeComponents.Size()<componentTypesCount) {
            objects.defaultObject.activeComponents.Resize(componentTypesCount);
            for(auto& o : objects.entries) {
                o.activeComponents.Resize(componentTypesCount);
                o.enabledComponents.Resize(componentTypesCount);
                o.componentIndicies.resize(componentTypesCount);
            }
        }
    }
    
    if (!components[componentId].container) {
        function(components[componentId]);
    }
}

void GameWorld::AddSystemType(SystemId systemId, const SystemTypeFunction& function) {
    if (systemId>=systems.size()) {
        systems.resize(systemId + 1);
    }
    
    SystemInfo& systemInfo = systems[systemId];
    
    if (!systemInfo.createFunction) {
        std::vector<ComponentId> componentIndices;
        function(systemInfo, componentIndices);
        Bitset systemBitset;
        for(auto c : componentIndices) {
            if (c>=systemBitset.Size()) {
                systemBitset.Resize(c + 1);
            }
            systemBitset.Set(c, true);
            components[c].systemsUsingComponent.push_back(systemId);
        }
        systemInfo.bitset = systemBitset;
    }
}

void GameWorld::RemoveSystemType(SystemId systemId) {
    SystemInfo& systemInfo = systems[systemId];
    if (!systemInfo.createFunction) return;
    scenes.Iterate([this, &systemInfo, systemId] (GameScene* scene) {
        IGameSystem* system = scene->systemsIndexed[systemId];
        objects.Iterate([&systemInfo, system] (GameObject* object) {
            if (systemInfo.bitset.Contains(object->enabledComponents)) {
                system->ObjectRemoved(object);
                system->RemoveObject(object);
            }
        });
        systemInfo.deleteFunction(system);
        scene->systemsIndexed[systemId] = 0;
        scene->activeSystems.erase(std::find(scene->activeSystems.begin(), scene->activeSystems.end(), system));
    });

    for(int i=0; i<systemInfo.bitset.Size(); ++i) {
        if (systemInfo.bitset[i]) {
            auto& list = components[i].systemsUsingComponent;
            list.erase(std::find(list.begin(), list.end(), systemId));
        }
    }
    systemInfo.createFunction = 0;
    systemInfo.deleteFunction = 0;
    systemInfo.bitset.Reset();
}

void GameWorld::DoActions(Actions &actions) {
    for(int i=0; i<actions.size();++i) {
        actions[i]();
    }
    actions.clear();
}

const ObjectCollection& GameWorld::Roots() { return roots; }

GameObject* GameWorld::CreateRoot() {
    int sceneIndex = scenes.Create(0);
    GameScene* scene = &scenes.entries[sceneIndex];
    scene->index=sceneIndex;
    scene->world = this;
    scene->guid = StringHelper::CreateGuid();
    activeScenes.push_back(scene);
    GameObject* root = CreateEmptyObject(0, scene, true);
    scene->root = root;
    roots.push_back(root);
    sortScenes = true;
    root->Order.Changed.Bind([this] () { sortScenes = true; });
    return root;
}

static std::function<void(GameObject*)> rootCreatedStatic = 0;

GameObject* GameWorld::CreateRootFromJson(std::istream &jsonStream,
                                       const std::function<void(GameObject*)>& rootCreated,
                                       const std::function<void(GameObject*)>& childCreated) {
    rootCreatedStatic = [&rootCreated](GameObject* o) {
        rootCreated(o);
    };
    GameObject* root = CreateObjectFromJson(0, jsonStream, childCreated);
    rootCreatedStatic = 0;
    return root;
}

void GameWorld::RemoveRoot(Pocket::GameObject *root) {
    GameScene* scene = root->scene;
    delayedActions.emplace_back([this, scene, root] {
        scene->DoActions(root->scene->delayedActions);
        roots.erase(std::find(roots.begin(), roots.end(), root));
        scene->DestroySystems();
        scenes.Delete(scene->index, 0);
        activeScenes.erase(std::find(activeScenes.begin(), activeScenes.end(), scene));
    });
}

void GameWorld::Update(float dt) {
    if (sortScenes) {
        sortScenes = false;
        std::sort(activeScenes.begin(), activeScenes.end(), [] (GameScene* a, GameScene* b) {
            return a->root->Order()<b->root->Order();
        });
    }
    
    for(auto scene : activeScenes) {
        scene->Update(dt);
    }
    DoActions(delayedActions);
}

void GameWorld::Render() {
    for(auto scene : activeScenes) {
        scene->Render();
    }
}

void GameWorld::Clear() {
   for(auto root : roots) {
        root->Remove();
        DoActions(root->scene->delayedActions);
   }
   DoActions(delayedActions);
}

int GameWorld::ObjectCount() { return objects.count; }

InputManager& GameWorld::Input() { return input; }

GameObject* GameWorld::CreateEmptyObject(GameObject *parent, GameScene* scene, bool assignId) {
    int index = objects.CreateNoInit(0);
    GameObject* object = &objects.entries[index];
    object->scene = scene;
    object->index = index;
    if (assignId) {
        object->rootId = ++scene->idCounter;
    }
    object->Reset();
    object->Parent = parent;
    return object;
}

GameObject* GameWorld::CreateObjectFromJson(Pocket::GameObject *parent, std::istream &jsonStream, const std::function<void (GameObject *)>& objectCreated) {
    minijson::istream_context context(jsonStream);
    GameObject* object = 0;
    try {
        object = LoadObject(parent, context, objectCreated);
        
        GameObject* object;
        int componentID;
        GameObject* referenceObject;
        while (GameObject::GetAddReferenceComponent(&object, componentID, &referenceObject)) {
            if (referenceObject) {
                object->AddComponent(componentID, referenceObject);
            }
        }
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
    }
    GameObject::EndGetAddReferenceComponent();
    return object;
}

GameObject* GameWorld::LoadObject(GameObject* parent, minijson::istream_context &context, const std::function<void(GameObject*)>& objectCreated) {
    GameObject* object = 0;
     minijson::parse_object(context, [&] (const char* n, minijson::value v) {
        if (v.type() == minijson::Object) {
            if (!parent) {
                object = CreateRoot();
                rootCreatedStatic(object);
            } else {
                object = CreateEmptyObject(parent, parent->scene, false);
            }
            minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                std::string name = n;
                if (name == "id" && v.type() == minijson::Number) {
                    object->rootId = (int)v.as_long();
                } else if (name == "Components" && v.type() == minijson::Array && object) {
                    minijson::parse_array(context, [&] (minijson::value v) {
                        if (v.type() == minijson::Object) {
                            minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                                object->AddComponent(context, n);
                            });
                        }
                    });
                } else if (name == "Children" && v.type() == minijson::Array && object) {
                    minijson::parse_array(context, [&] (minijson::value v) {
                        LoadObject(object, context, objectCreated);
                    });
                } else if (!parent && name == "guid" && v.type() == minijson::String) {
                    object->scene->guid = std::string(v.as_string());
                } else if (!parent && name == "counter" && v.type() == minijson::Number) {
                    object->scene->idCounter = (int)v.as_long();
                }
                
                if (objectCreated) {
                    objectCreated(object);
                }
            });
        }
    });
    return object;
}

bool GameWorld::TryGetComponentIndex(const std::string& componentName, int& index) {
    for(int i=0; i<components.size(); ++i) {
        if (components[i].name == componentName) {
            index = i;
            return true;
        }
    }
    return false;
}

bool GameWorld::TryGetComponentIndex(const std::string& componentName, int& index, bool& isReference) {
    if (TryGetComponentIndex(componentName, index)) {
        isReference = false;
        return true;
    }
    if (componentName.size()>4) {
        size_t refLocation = componentName.rfind(":ref");
        if (refLocation!=-1) {
            std::string componentNameNoRef = componentName.substr(0, refLocation);
            if (TryGetComponentIndex(componentNameNoRef, index)) {
                isReference = true;
                return true;
            }
        }
    }
    return false;
}

GameScene* GameWorld::TryGetScene(const std::string &guid) {
    for(auto s : activeScenes) {
        if (s->guid == guid) return s;
    }
    return 0;
}

GameObject* GameWorld::FindObject(const std::string &guid, int objectId) {
    GameScene* scene = TryGetScene(guid);
    return scene->FindObject(objectId);
}

ComponentTypeCollection GameWorld::GetComponentTypes() {
    ComponentTypeCollection componentTypes;
    for(auto& c : components) {
        componentTypes.push_back({ c.name, c.getTypeInfo });
    }
    return componentTypes;
}