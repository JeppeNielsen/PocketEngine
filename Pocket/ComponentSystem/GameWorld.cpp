//
//  GameWorld.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 17/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameWorld.hpp"
#include "GameStorage.hpp"
#include "StringHelper.hpp"
#include "GameObjectHandle.hpp"

using namespace Pocket;

GameWorld::GameWorld() : storage(nullptr) { }

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
                if (a.system->GetIndex() == b.system->GetIndex()) {
                    return a.scene->index<b.scene->index;
                } else {
                    return a.system->GetIndex()<b.system->GetIndex();
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

const ObjectCollection& GameWorld::Scenes() { return sceneRoots; }

GameObject* GameWorld::CreateScene() {
    int sceneIndex = storage->scenes.Create(0);
    GameScene* scene = &storage->scenes.entries[sceneIndex];
    scene->world = this;
    scene->Initialize(this, storage, sceneIndex);
    sceneRoots.push_back(scene->root);
    SceneCreated(scene->root);
    return scene->root;
}

GameObject* GameWorld::CreateScene(GameObject* prefab) {
    GameObject* scene = CreateScene();
    scene->ApplyClone(prefab);
    return scene;
}

GameObject* GameWorld::FindScene(const std::string& guid) {
    for(auto scene : sceneRoots) {
        if (scene->scene->guid == guid) return scene;
    }
    return nullptr;
}

void GameWorld::RemoveScene(Pocket::GameObject *sceneRoot) {
    GameScene* scene = sceneRoot->scene;
    delayedActions.emplace_back([this, scene, sceneRoot] {
        sceneRoots.erase(std::find(sceneRoots.begin(), sceneRoots.end(), sceneRoot));
        scene->Destroy();
        storage->scenes.Delete(scene->index, 0);
        SceneRemoved(sceneRoot);
    });
}

void GameWorld::Update(float dt) {
    DoActions(delayedActions);
    for(auto& s : activeSystems) {
        s.system->Update(dt);
    }
}

void GameWorld::Render() {
    for(auto&s : activeSystems) {
        s.system->Render();
    }
}

void GameWorld::Clear() {
    for(auto scene : sceneRoots) {
        scene->Remove();
    }
    DoActions(delayedActions);
}

InputManager& GameWorld::Input() { return input; }

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

GameStorage& GameWorld::Storage() const {
    return *storage;
}
