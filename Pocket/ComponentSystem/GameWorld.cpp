//
//  GameWorld.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 17/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameWorld.hpp"
#include "StringHelper.hpp"
#include "GameObjectHandle.hpp"

using namespace Pocket;

GameWorld::GameWorld() : componentTypesCount(0) {
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

void GameWorld::AddActiveSystem(IGameSystem *system, GameScene *scene) {
    activeSystems.push_back({system, scene});
    SortActiveSystems();
}

void GameWorld::RemoveActiveSystem(Pocket::IGameSystem *system) {
    activeSystems.erase(std::find(activeSystems.begin(), activeSystems.end(), ActiveSystem{ system, 0 }));
}

void GameWorld::SortActiveSystems() {
    std::sort(activeSystems.begin(), activeSystems.end(), [] (ActiveSystem& a, ActiveSystem& b) {
        if (a.system->GetOrder() == b.system->GetOrder()) {
            if (a.scene == b.scene) {
                return a.system->GetIndex()<b.system->GetIndex();
            } else {
                if (a.scene->root->Order() == b.scene->root->Order()) {
                    if (a.system->GetIndex() == b.system->GetIndex()) {
                        return a.scene->index<b.scene->index;
                    } else {
                        return a.system->GetIndex()<b.system->GetIndex();
                    }
                } else {
                    return a.scene->root->Order()<b.scene->root->Order();
                }
            }
        } else {
            return a.system->GetOrder()<b.system->GetOrder();
        }
    });
}

void GameWorld::RemoveSystemType(SystemId systemId) {
    SystemInfo& systemInfo = systems[systemId];
    if (!systemInfo.createFunction) return;
    scenes.Iterate([this, &systemInfo, systemId] (GameScene* scene) {
        if (systemId>=scene->systemsIndexed.size()) return;
        IGameSystem* system = scene->systemsIndexed[systemId];
        if (!system) return;
        if (system->ObjectCount()>0) {
            scene->IterateObjects([&systemInfo, system] (GameObject* object) {
                if ( systemInfo.bitset.Contains(object->enabledComponents)) {
                    system->ObjectRemoved(object);
                    system->RemoveObject(object);
                }
            });
            RemoveActiveSystem(system);
        }
        systemInfo.deleteFunction(system);
        scene->systemsIndexed[systemId] = 0;
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
        roots.erase(std::find(roots.begin(), roots.end(), root));
        scene->DestroySystems();
        scenes.Delete(scene->index, 0);
        activeScenes.erase(std::find(activeScenes.begin(), activeScenes.end(), scene));
        RootRemoved(root);
    });
}

void GameWorld::Update(float dt) {
    UpdateActions();
    for(auto& s : activeSystems) {
        if (!s.scene->updateEnabled()) continue;
        s.system->Update(s.scene->timeScale() * dt);
    }
}

void GameWorld::UpdateRoot(float dt, GameObject* root) {
    UpdateActions();
    for(auto& s : activeSystems) {
        if (s.scene==root->scene) {
            s.system->Update(s.scene->timeScale() * dt);
        }
    }
}

void GameWorld::UpdateActions() {
    DoActions(delayedActions);
}

void GameWorld::DebugSystems() {
    std::cout <<"-------------------------------------------"<<std::endl;
    for(auto&s : activeSystems) {
        std::cout << systems[s.system->GetIndex()].name<<" Order: "<<s.system->GetOrder()<< " index: " << s.system->GetIndex() <<" scene: "<<s.scene->index<<"  root->order: "<<s.scene->root->Order() << " Num objects:"<< s.system->ObjectCount() << std::endl;
    }
}

void GameWorld::Render() {
    for(auto&s : activeSystems) {
        if (s.scene->renderEnabled) {
            s.system->Render();
        }
    }
}

void GameWorld::Clear() {
    GuidToRoot = 0;
    GuidToPath = 0;
    GetPaths = 0;
    for(auto root : roots) {
        root->Remove();
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
    if (scene->ObjectCreated) {
        scene->ObjectCreated(object);
    }
    return object;
}

GameObject* GameWorld::CreateObjectFromJson(Pocket::GameObject *parent, std::istream &jsonStream, const std::function<void (GameObject *)>& objectCreated) {
    minijson::istream_context context(jsonStream);
    GameObject* object = 0;
    try {
        GameObject::AddReferenceComponentList addReferenceComponents;
        object = LoadObject(addReferenceComponents, parent, context, objectCreated);
        
        GameObject* object;
        int componentID;
        GameObject* referenceObject;
        while (GameObject::GetAddReferenceComponent(addReferenceComponents, &object, componentID, &referenceObject)) {
            if (referenceObject) {
                object->AddComponent(componentID, referenceObject);
            }
        }
    } catch (minijson::parse_error e) {
        std::cout << e.what() << std::endl;
    }
    GameObject::EndGetAddReferenceComponent();
    return object;
}

GameObject* GameWorld::LoadObject(GameObject::AddReferenceComponentList& addReferenceComponents, GameObject* parent, minijson::istream_context &context, const std::function<void(GameObject*)>& objectCreated) {
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
                                object->AddComponent(addReferenceComponents, context, n);
                            });
                        }
                    });
                } else if (name == "Children" && v.type() == minijson::Array && object) {
                    minijson::parse_array(context, [&] (minijson::value v) {
                        LoadObject(addReferenceComponents, object, context, objectCreated);
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

GameObject* GameWorld::TryFindRoot(const std::string &guid) {
    GameScene* scene = TryGetScene(guid);
    if (!scene) return 0;
    return scene->root;
}

GameScene* GameWorld::TryGetScene(const std::string &guid) {
    for(auto s : activeScenes) {
        if (s->guid == guid) return s;
    }
    if (GuidToRoot) {
        GameObject* root = GuidToRoot(guid);
        if (root && root->IsRoot()) {
            return root->scene;
        }
    }
    return 0;
}

std::string GameWorld::TryFindScenePath(const std::string &guid) {
    if (!GuidToPath) { return ""; }
    return GuidToPath(guid);
}

GameObject* GameWorld::FindObject(const std::string &guid, int objectId) {
    GameScene* scene = TryGetScene(guid);
    return scene->FindObject(objectId);
}

ComponentTypeCollection GameWorld::GetComponentTypes() const {
    ComponentTypeCollection componentTypes;
    for(auto& c : components) {
        componentTypes.push_back({ c.name, c.getTypeInfo });
    }
    return componentTypes;
}

std::string GameWorld::ReadGuidFromJson(std::istream &jsonStream) {
    std::string guid;
    minijson::istream_context context(jsonStream);
    
    try {
        minijson::parse_object(context, [&] (const char* n, minijson::value v) {
            if (v.type() == minijson::Object) {
                minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                    std::string name = n;
                    if (name == "guid" && v.type() == minijson::String) {
                        guid = std::string(v.as_string());
                    } else {
                        minijson::ignore(context);
                    }
                });
            } else {
                minijson::ignore(context);
            }
        });
    } catch (minijson::parse_error e) {
        std::cout << e.what() << std::endl;
    }
    return guid;
}

void GameWorld::TryParseJson(std::istream &jsonStream, int componentId,
    const std::function<void (int, int)>& callback,
    const std::function<bool (const std::string& componentName)>& componentCallback) {
    minijson::istream_context context(jsonStream);
    try {
        std::string componentName = componentId<0 ? "" : components[componentId].name;
        TryParseJsonObject(-1, context, componentName, callback, componentCallback);
    } catch (minijson::parse_error e) {
        std::cout << e.what() << std::endl;
    }
}

void GameWorld::TryParseJsonObject(int parent, minijson::istream_context &context, const std::string& componentName,
                                   const std::function<void (int, int)>& callback,
                                   const std::function<bool (const std::string& componentName)>& componentCallback) {
     bool recurseChildren = true;
     int objectId = -1;
     minijson::parse_object(context, [&] (const char* n, minijson::value v) {
        if (v.type() == minijson::Object) {
            minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                std::string name = n;
                if (name == "id" && v.type() == minijson::Number) {
                    objectId = (int)v.as_long();
                } else if (name == "Components" && v.type() == minijson::Array && objectId!=-1) {
                    minijson::parse_array(context, [&] (minijson::value v) {
                        if (v.type() == minijson::Object) {
                            minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                                std::string name = n;
                                if (componentName == name && componentName!="") {
                                    callback(parent, objectId);
                                }
                                if (componentCallback && !componentCallback(name)) {
                                    recurseChildren = false;
                                }
                                minijson::ignore(context);
                            });
                        } else {
                            minijson::ignore(context);
                        }
                    });
                } else if (name == "Children" && v.type() == minijson::Array && objectId!=-1) {
                    if (recurseChildren) {
                        minijson::parse_array(context, [&] (minijson::value v) {
                            TryParseJsonObject(objectId, context, componentName, callback, componentCallback);
                        });
                    }
                } else {
                    minijson::ignore(context);
                }
                if (componentName=="") {
                    callback(parent, objectId);
                }
            });
        } else {
            minijson::ignore(context);
        }
    });
}

void GameWorld::InvokeChangeToHandles(Pocket::GameObject *object) {

    std::vector<GameObjectHandle*> handlesToChange;
    
    const std::string& id = object->RootGuid();
    
    for(GameObjectHandle* h : handles) {
        if (h->sceneGuid == id) {
            //std::cout << "GameObjectHandle invoked : guid : " << h->sceneGuid << " index " << h->index << " rootId "<<h->rootId << std::endl;
            handlesToChange.push_back(h);
            //h->Changed();
        }
    }
    
    for(auto h : handlesToChange) {
        h->Changed();
    }
}

Container<GameScene>& GameWorld::Scenes() { return scenes; }

void GameWorld::SetLayerScene(int layerNo, Pocket::GameObject *scene) {
    auto it = sceneLayers.find(layerNo);
    if (it!=sceneLayers.end()) {
        it->second->Remove();
        if (!scene) {
            sceneLayers.erase(it);
        }
    }
    if (scene) {
        sceneLayers[layerNo] = scene;
    }
    LayersChanged();
}

const GameWorld::SceneLayers& GameWorld::GetSceneLayers() { return sceneLayers; }

void GameWorld::SerializeAndRemoveComponents(std::ostream& stream, const SerializePredicate &predicate) {
    minijson::writer_configuration config;
    config = config.pretty_printing(true);
    minijson::object_writer writer(stream, config);
    objects.Iterate([&writer, &predicate](GameObject* object) {
        object->WriteJsonComponents(writer, predicate);
        object->RemoveComponents(predicate);
    });
    writer.close();
}

void GameWorld::DeserializeAndAddComponents(std::istream &jsonStream) {
    minijson::istream_context context(jsonStream);

    try {
        GameObject::AddReferenceComponentList addReferenceComponents;

        std::vector<GameObject*> objectsList;
        objects.Iterate([&](GameObject* object) {
            objectsList.push_back(object);
        });
    
        int index = 0;
        minijson::parse_object(context, [&] (const char* n, minijson::value v) {
            GameObject* object = objectsList[index];
            index++;
            if (v.type() == minijson::Array) {
                minijson::parse_array(context, [&] (minijson::value v) {
                    if (v.type() == minijson::Object) {
                        minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                            object->AddComponent(addReferenceComponents, context, n);
                        });
                    }
                });
            }
        });
        
        GameObject* object;
        int componentID;
        GameObject* referenceObject;
        while (GameObject::GetAddReferenceComponent(addReferenceComponents, &object, componentID, &referenceObject)) {
            if (referenceObject) {
                object->AddComponent(componentID, referenceObject);
            }
        }
    } catch (minijson::parse_error e) {
        std::cout << e.what() << std::endl;
    }
    GameObject::EndGetAddReferenceComponent();
}
