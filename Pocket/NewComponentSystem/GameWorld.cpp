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

const ObjectCollection GameWorld::Roots() { return roots; }

GameObject* GameWorld::CreateRoot() {

    int sceneIndex = scenes.Create();
    GameScene* scene = &scenes.entries[sceneIndex];
    scene->index=sceneIndex;
    scene->world = this;
    activeScenes.push_back(scene);

    int index = objects.CreateNoInit();
    GameObject* root = &objects.entries[index];
    root->scene = scene;
    root->index = index;
    root->Reset();
    scene->root = root;
    roots.push_back(root);
    
    return root;
}

void GameWorld::Update(float dt) {
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
        root->scene->DoActions(root->scene->delayedActions);
   }
   DoActions(delayedActions);
}

void GameWorld::RemoveRoot(Pocket::GameObject *root) {
    delayedActions.emplace_back([this, root] {
        roots.erase(std::find(roots.begin(), roots.end(), root));
        GameScene* scene = root->scene;
        scene->DestroySystems();
        scenes.Delete(scene->index);
        activeScenes.erase(std::find(activeScenes.begin(), activeScenes.end(), scene));
    });
}
