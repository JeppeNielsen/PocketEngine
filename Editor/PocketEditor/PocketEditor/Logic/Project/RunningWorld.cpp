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
        }
    });
}

void RunningWorld::Initialize(const std::string &path, const std::vector<std::string> &startScenes, ScriptWorld& scriptWorld) {

    world.RootRemoved.Bind([this] (GameObject* root) {
        if (ActiveScene == root) {
            editorScene.Destroy();
        }
    });

    fileWorld.AddGameWorld(world);
    fileWorld.FindRoots(path, {".json", ".meta" });
    fileWorld.OnRootCreated = [this, &scriptWorld] (GameObject* root) {
        SystemHelper::AddGameSystems(*root);
        
        scriptWorld.AddGameRoot(root);
    };
    //world.AddComponentType<Cloner>();
    //world.AddComponentType<CloneVariable>();
    //world.AddComponentType<EditorDropTarget>();
    //world.AddComponentType<ProjectSettings>();
    
    GameObject* initRoot = world.CreateRoot();
    SystemHelper::AddGameSystems(*initRoot);
    //OpenWorld::CreateEditorSystems(*initRoot);
    initRoot->CreateSystem<AssetManager>();
    initRoot->Remove();
    scriptWorld.AddGameWorld(world);
    
    int layerNo = 0;
    for(auto& startScene : startScenes) {
        world.SetLayerScene(layerNo, world.TryFindRoot(startScene));
        layerNo++;
    }
    this->scriptWorld = &scriptWorld;
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
    world.SerializeAndRemoveComponents(serializedComponents, [this] (const GameObject* o, int componentId) {
        return componentId>=scriptWorld->GetBaseComponentIndex();
    });
    
    world.UpdateActions();
    scriptWorld->RemoveGameWorld(world);
    
    std::cout << serializedComponents.str() << std::endl;
}

void RunningWorld::PostCompile() {
    scriptWorld->AddGameWorld(world);
    scriptWorld->AddAllGameRoots(world);
    world.DeserializeAndAddComponents(serializedComponents);
    world.UpdateActions();
    serializedComponents.clear();
}
