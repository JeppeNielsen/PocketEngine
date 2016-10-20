//
//  GameWorld.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 17/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameWorld.hpp"

using namespace Pocket;

GameWorld::GameWorld() : componentTypesCount(0) {
    scenes.defaultObject.world = this;
}
GameWorld::~GameWorld() { Clear(); }

void GameWorld::AddComponentType(ComponentId componentId, const ComponentTypeFunction& function) {
    if (componentId>=components.size()) {
        components.resize(componentId + 1);
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
            if (c>=componentTypesCount) {
                componentTypesCount = c + 1;
            }
        }
        systemInfo.bitset = systemBitset;
        objects.defaultObject.activeComponents.Resize(componentTypesCount);
    }
}

void GameWorld::DoActions(Actions &actions) {
    for(int i=0; i<actions.size();++i) {
        actions[i]();
    }
    actions.clear();
}

const SceneCollection GameWorld::Scenes() { return scenes.GetCollection(); }

GameScene* GameWorld::CreateScene() {
    int index = scenes.CreateNoInit();
    GameScene* scene = &scenes.entries[index];
    scene->removed = false;
    scene->world = this;
    scene->index = index;
    scene->Active = true;
    return scene;
}

void GameWorld::Update(float dt) {
    DoActions(delayedActions);
    for(auto scene : activeScenes) {
        scene->Update(dt);
    }
}

void GameWorld::Render() {
    for(auto scene : activeScenes) {
        scene->Render();
    }
}

void GameWorld::Clear() {
   scenes.Iterate([] (GameScene* scene) {
        scene->Remove();
   });
   DoActions(delayedActions);
}