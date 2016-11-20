//
//  GameScene.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 18/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameScene.hpp"
#include "GameWorld.hpp"
#include "StringHelper.hpp"

using namespace Pocket;

GameScene::GameScene() : idCounter(0) {
    updateEnabled = true;
    renderEnabled = true;
    timeScale = 1.0f;
}

GameScene::~GameScene() {
    for (int i=0; i<systemsIndexed.size(); ++i) {
        if (!systemsIndexed[i]) continue;
        if (world->systems[i].deleteFunction) {
            world->systems[i].deleteFunction(systemsIndexed[i]);
        } else {
            delete systemsIndexed[i];
        }
    }
}

GameScene::GameScene(const GameScene& other) {
    systemsIndexed.clear();
    updateEnabled = true;
    renderEnabled = true;
    timeScale = 1.0f;
}

void GameScene::DestroySystems() {
    for (int i=0; i<systemsIndexed.size(); ++i) {
        if (systemsIndexed[i]) {
            systemsIndexed[i]->Destroy();
        }
    }
}

void GameScene::Render() {
    if (!renderEnabled()) return;
}

GameObject* GameScene::FindObject(int objectId) {
    std::vector<GameObject*> nodesToVisit;
    nodesToVisit.push_back(root);
    
    while (!nodesToVisit.empty()) {
        GameObject* current = nodesToVisit.back();
        if (current->rootId == objectId) return current;
        nodesToVisit.pop_back();
        nodesToVisit.insert(nodesToVisit.begin(), current->Children().begin(), current->Children().end());
    }
    
    return 0;
}

void GameScene::IterateObjects(const std::function<void (GameObject *)> &callback) {
    std::vector<GameObject*> nodesToVisit;
    nodesToVisit.push_back(root);
    
    while (!nodesToVisit.empty()) {
        GameObject* current = nodesToVisit.back();
        callback(current);
        nodesToVisit.pop_back();
        nodesToVisit.insert(nodesToVisit.end(), current->Children().begin(), current->Children().end());
    }
}

IGameSystem* GameScene::CreateSystem(int systemId) {
    if (systemId>=systemsIndexed.size()) {
        systemsIndexed.resize(systemId + 1, 0);
    }

    if (!systemsIndexed[systemId]) {
        IGameSystem* system = world->systems[systemId].createFunction(root);
        systemsIndexed[systemId] = system;
        system->index = systemId;
        system->Initialize();
        system->Order.Changed.Bind([this] () { world->delayedActions.emplace_back([this] () { world->SortActiveSystems(); }); });
        
        IterateObjects([systemId](GameObject* object) {
            object->TryAddToSystem(systemId);
        });
    }
    return systemsIndexed[systemId];
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
    
    if (world->systems[systemId].deleteFunction) {
        world->systems[systemId].deleteFunction(system);
    } else {
        delete system;
    }
    systemsIndexed[systemId] = 0;
}
