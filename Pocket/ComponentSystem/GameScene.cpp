//
//  GameScene.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 18/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameScene.hpp"
#include "StringHelper.hpp"
#include "GameWorld.hpp"
#include "GameStorage.hpp"

using namespace Pocket;

GameScene::GameScene() : idCounter(0),
	ObjectCreated(0), ObjectRemoved(0), ComponentCreated(0), ComponentRemoved(0) {
}

GameScene::~GameScene() {
    Destroy();
}

void GameScene::Initialize(GameWorld* world, GameStorage* storage, int index) {
    this->world = world;
    this->storage = storage;
    this->index = index;
    
    guid = StringHelper::CreateGuid();
    root = CreateEmptyObject(nullptr, true);
    for (int i=0; i<this->storage->systems.size(); ++i) {
        if (this->storage->systems[i].createFunction) {
            CreateSystem(i);
        }
    }
}

void GameScene::Destroy() {
    for (int i=0; i<systemsIndexed.size(); ++i) {
        if (systemsIndexed[i]) {
            systemsIndexed[i]->Destroy();
            if (storage->systems[i].deleteFunction) {
                storage->systems[i].deleteFunction(systemsIndexed[i]);
            } else {
                delete systemsIndexed[i];
            }
        }
    }
    systemsIndexed.clear();
}

GameObject* GameScene::FindObject(int objectId) {
    std::vector<GameObject*> nodesToVisit;
    nodesToVisit.push_back(root);
    
    while (!nodesToVisit.empty()) {
        GameObject* current = nodesToVisit.back();
        if (current->id == objectId) return current;
        nodesToVisit.pop_back();
        Hierarchy& h = current->Hierarchy();
        nodesToVisit.insert(nodesToVisit.begin(), h.Children().begin(), h.Children().end());
    }
    
    return nullptr;
}

void GameScene::IterateObjects(const std::function<void (GameObject *)> &callback) {
    std::vector<GameObject*> nodesToVisit;
    nodesToVisit.push_back(root);
    
    while (!nodesToVisit.empty()) {
        GameObject* current = nodesToVisit.back();
        callback(current);
        nodesToVisit.pop_back();
        Hierarchy& h = current->Hierarchy();
        nodesToVisit.insert(nodesToVisit.end(), h.Children().begin(), h.Children().end());
    }
}

void GameScene::CreateSystem(int systemId) {
    if (systemId>=systemsIndexed.size()) {
        systemsIndexed.resize(systemId + 1, nullptr);
    }

    if (!systemsIndexed[systemId]) {
        IGameSystem* system = storage->systems[systemId].createFunction(root);
        systemsIndexed[systemId] = system;
        system->SetIndex(systemId);
        system->Initialize();
        IterateObjects([systemId](GameObject* object) {
            object->TryAddToSystem(systemId);
        });
    }
}

void GameScene::RemoveSystem(int systemId) {
    if (systemId>=systemsIndexed.size()) {
        return;
    }
    IGameSystem* system = systemsIndexed[systemId];
    if (!system) return;
    
    IterateObjects([systemId](GameObject* object) {
        object->TryRemoveFromSystem(systemId);
    });
    system->Destroy();
    
    if (storage->systems[systemId].deleteFunction) {
        storage->systems[systemId].deleteFunction(system);
    } else {
        delete system;
    }
    systemsIndexed[systemId] = 0;
}

GameObject* GameScene::CreateEmptyObject(GameObject *parent, bool assignId) {
    int index = storage->objects.CreateNoInit(0);
    GameObject* object = &storage->objects.entries[index];
    object->scene = this;
    object->index = index;
    if (assignId) {
        object->id = ++idCounter;
    }
    object->Reset();
    object->Hierarchy().Parent = parent;
    if (ObjectCreated) {
        ObjectCreated(object);
    }
    return object;
}


