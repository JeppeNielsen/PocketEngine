//
//  SceneLayerSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 25/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "SceneManagerSystem.hpp"

using namespace Pocket;

void SceneManagerSystem::Initialize() {
    layersAreChanging = false;
    //root->World()->LayersChanged.Bind(this, &SceneManagerSystem::LayersChanged);
}

void SceneManagerSystem::Destroy() {
    //root->World()->LayersChanged.Unbind(this, &SceneManagerSystem::LayersChanged);
}

void SceneManagerSystem::LayersChanged() {

    /*std::map<int, GameObjectHandle> activeScenes;
    auto& sceneLayers = root->World()->GetSceneLayers();
    for(auto& s : sceneLayers) {
        activeScenes[s.first] = GameObjectHandle(s.second);
    }

    layersAreChanging = true;
    std::map<int, GameObjectHandle> empty;
    for(GameObject* object : Objects()) {
        SceneManager* sceneManager = object->GetComponent<SceneManager>();
        sceneManager->ActiveScenes = activeScenes;
        sceneManager->SetScenes = empty;
    }
    layersAreChanging = false;
    */
}

void SceneManagerSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<SceneManager>()->SetScenes.Changed.Bind(this, &SceneManagerSystem::ScenesSet, object);
}

void SceneManagerSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<SceneManager>()->SetScenes.Changed.Unbind(this, &SceneManagerSystem::ScenesSet, object);}

void SceneManagerSystem::ScenesSet(Pocket::GameObject *object) {
    if (layersAreChanging) return;
    
    SceneManager* sceneManager = object->GetComponent<SceneManager>();
    auto scenesSet = sceneManager->SetScenes();
    
    for(auto s : scenesSet) {
        auto& handle = s.second;
        handle.SetStorage(&root->World()->Storage());
        //root->World()->SetLayerScene(s.first, handle());
    }
}
