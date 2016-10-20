//
//  GameScene.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 18/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameScene.hpp"
#include "GameWorld.hpp"

using namespace Pocket;

GameScene::GameScene() { }

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
    activeSystems.clear();
    systemsIndexed.resize(other.world->systems.size(), 0);
    delayedActions.clear();
    
    for (int i=0; i<other.world->systems.size(); ++i) {
        if (other.world->systems[i].createFunction) {
            systemsIndexed[i] = other.world->systems[i].createFunction();
        }
    }
    
    Active.Changed.Bind([this] {
        if (Active()) {
            world->delayedActions.emplace_back([this]() {
               world->activeScenes.push_back(this);
            });
        } else {
            world->delayedActions.emplace_back([this]() {
               std::find(world->activeScenes.begin(), world->activeScenes.end(), this);
            });
        }
    });
}

void GameScene::DoActions(Actions &actions) {
    for(int i=0; i<actions.size();++i) {
        actions[i]();
    }
    actions.clear();
}

void GameScene::Update(float dt) {
    DoActions(delayedActions);
    for(auto system : activeSystems) {
        system->Update(dt);
    }
}

void GameScene::Render() {
    for(auto system : activeSystems) {
        system->Render();
    }
}

const GameObject* GameScene::Root() { return &root; }

GameObject* GameScene::CreateObject() {
    auto& worldObjects = world->objects;
    int index = worldObjects.CreateNoInit();
    GameObject* object = &worldObjects.entries[index];
    object->scene = this;
    object->index = index;
    object->Reset();
    object->Parent = &root;
    return object;
}

void GameScene::Remove() {
    if (removed) return;
    removed = true;
    Clear();
    world->delayedActions.emplace_back([this] () {
        world->scenes.Delete(index);
    });
}

void GameScene::Clear() {
    world->delayedActions.emplace_back([this] () {
        world->objects.Iterate([this](GameObject* object) {
            if (object->scene != this) return;
            object->Remove();
        });
        DoActions(delayedActions);
    });
}

Handle<GameScene> GameScene::GetHandle() {
    return world->scenes.GetHandle(index);
}