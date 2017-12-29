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

GameWorld::GameWorld() : storage(nullptr) {
    scenes.defaultObject.world = this;
}

GameWorld::GameWorld(GameStorage& storage) : GameWorld() {
    Initialize(storage);
}

GameWorld::~GameWorld() { Clear(); }

void GameWorld::Initialize(GameStorage& storage) {
    this->storage = &storage;
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
    scene->storage = storage;
    
    scene->index=sceneIndex;
    scene->world = this;
    scene->guid = StringHelper::CreateGuid();
    activeScenes.push_back(scene);
    GameObject* root = scene->CreateEmptyObject(0, scene, true);
    scene->root = root;
    roots.push_back(root);
    for (int i=0; i<storage->systems.size(); ++i) {
        if (storage->systems[i].createFunction) {
            scene->CreateSystem(i);
        }
    }
    RootCreated(root);
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
//    for(auto&s : activeSystems) {
//        std::cout << systems[s.system->GetIndex()].name<<" Order: "<<s.system->GetOrder()<< " index: " << s.system->GetIndex() <<" scene: "<<s.scene->index<<"  root->order: "<<s.scene->root->Order() << " Num objects:"<< s.system->ObjectCount() << std::endl;
//    }
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

InputManager& GameWorld::Input() { return input; }


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


