//
//  SceneLayerSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 25/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "SceneManagerSystem.hpp"

using namespace Pocket;

void SceneManagerSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<SceneManager>()->ActiveScenes.Changed.Bind(this, &SceneManagerSystem::ActiveScenesChanged, object);
    ActiveScenesChanged(object);
}

void SceneManagerSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<SceneManager>()->ActiveScenes.Changed.Unbind(this, &SceneManagerSystem::ActiveScenesChanged, object);
}

void SceneManagerSystem::ActiveScenesChanged(Pocket::GameObject *object) {
    SceneManager* sceneManager = object->GetComponent<SceneManager>();
    auto old = sceneManager->ActiveScenes.PreviousValue();
    
    auto current = sceneManager->ActiveScenes();
    
    for(auto it : old) {
        bool isNotInCurrent = current.find(it.first) == current.end();
        if (isNotInCurrent) {
            sceneManager->sceneRoots[it.first]->Remove();
            sceneManager->sceneRoots.erase(sceneManager->sceneRoots.find(it.first));
        }
    }
    
    for(auto it : current) {
        bool isNotInOld = old.find(it.first) == old.end();
        if (isNotInOld) {
            it.second.SetRoot(root);
            sceneManager->sceneRoots[it.first] = it.second.operator()();
        }
    }
}
