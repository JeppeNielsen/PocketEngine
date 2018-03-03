//
//  RunningWorld.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 01/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "RunningWorld.hpp"
#include "OpenWorld.hpp"
#include "AssetManager.hpp"
#include "ScriptWorld.hpp"
#include "Cloner.hpp"
#include "CloneVariable.hpp"
#include "EditorDropTarget.hpp"
#include "ProjectSettings.hpp"
#include "SystemHelper.hpp"

RunningWorld::RunningWorld() {
    ActiveScene = 0;
    ActiveScene.Changed.Bind([this] () {
        editorScene.Destroy();
        if (ActiveScene) {
            editorScene.Initialize(ActiveScene);
            
        } else {
            
        }
    });
}

void RunningWorld::Initialize(const std::string &path, const std::vector<std::string> &startScenes, GameStorage& storage, ScriptWorld& scriptWorld) {

    world.Initialize(storage);
    world.SceneRemoved.Bind([this] (GameObject* scene) {
        if (ActiveScene == scene) {
            editorScene.Destroy();
        }
    });
    
    /*int layerNo = 0;
    for(auto& startScene : startScenes) {
        world.SetLayerScene(layerNo, world.TryFindRoot(startScene));
        layerNo++;
    }
    */
    
    GameObject* prefab = storage.TryGetPrefab(startScenes[0]);
    GameObject* scene = world.CreateScene();
    SystemHelper::AddGameSystems(*scene);
    scriptWorld.AddGameScene(scene);
    scene->ApplyClone(prefab);
}

GameWorld& RunningWorld::World() {
    return world;
}

void RunningWorld::Destroy() {
    editorScene.Destroy();
}

GameObject* RunningWorld::EditorRoot() {
    return editorScene.EditorRoot();
}

void RunningWorld::PreCompile() {

}

void RunningWorld::PostCompile() {

}
