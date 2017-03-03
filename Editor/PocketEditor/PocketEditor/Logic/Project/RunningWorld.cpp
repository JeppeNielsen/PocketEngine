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

void RunningWorld::Initialize(const std::string &path, const std::vector<std::string> &startScenes, ScriptWorld& scriptWorld) {
    fileWorld.AddGameWorld(world);
    fileWorld.FindRoots(path, {".json", ".meta" });
    fileWorld.OnRootCreated = [&scriptWorld] (GameObject* root) {
        OpenWorld::CreateDefaultSystems(*root);
        root->CreateSystem<AssetManager>();
        scriptWorld.AddGameRoot(root);
    };
    //world.AddComponentType<Cloner>();
    //world.AddComponentType<CloneVariable>();
    //world.AddComponentType<EditorDropTarget>();
    //world.AddComponentType<ProjectSettings>();
    
    GameObject* initRoot = world.CreateRoot();
    OpenWorld::CreateDefaultSystems(*initRoot);
    //OpenWorld::CreateEditorSystems(*initRoot);
    initRoot->CreateSystem<AssetManager>();
    initRoot->Remove();
    scriptWorld.AddGameWorld(world);
    
    int layerNo = 0;
    for(auto& startScene : startScenes) {
        world.SetLayerScene(layerNo, world.TryFindRoot(startScene));
        layerNo++;
    }
}

GameWorld& RunningWorld::World() {
    return world;
}
