//
//  OpenWorld.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 13/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "OpenWorld.hpp"
#include "FileHelper.hpp"
#include "ScriptWorld.hpp"
#include <fstream>
#include "Project.hpp"
#include "Cloner.hpp"
#include "EditorContext.hpp"
#include "SystemHelper.hpp"

OpenWorld::OpenWorld() : runningWorld(nullptr) {
    IsPlaying = false;
    EditorRoot = 0;
}

bool IsClonerInAncestry(GameObject* object) {
    while(true) {
        object = object->Hierarchy().Parent;
        if (!object) return false;
        if (object->HasComponent<Cloner>()) return true;
    }
}

bool OpenWorld::Save() {
    Stop();
    bool succes = false;
    try {
        succes = true;
        std::ofstream file;
        file.open(Path);
        
        context->Storage().ApplyPrefab(prefab, scene);
        serializer.Serialize(prefab, file, [] (const GameObject* go, int componentID) {
            if (componentID == Pocket::GameIdHelper::GetComponentID<EditorObject>()) return false;
            if (go->Hierarchy().Parent() && go->Hierarchy().Parent()->GetComponent<Cloner>()) return false;
            return true;
        });
        file.close();
    } catch (std::exception e) {
        succes = false;
    }
    return succes;
}

bool OpenWorld::Load(const std::string &path, const std::string &filename, EditorContext* context) {
    Path = path;
    Filename = filename;
    this->context = context;
    
    if (path != "") {
        std::ifstream file;
        
        file.open(path);
        std::string guid = serializer.ReadGuidFromJson(file);
        file.close();
        
        prefab = context->Storage().TryGetPrefab(guid);
        if (!prefab) {
            return false;
        }
        world.Initialize(context->Storage());
        scene = world.CreateScene();
        SystemHelper::AddGameSystems(*scene);
        editorScene.Initialize(scene);
        scene->ApplyClone(prefab);
    } else {
        return false;
    }
    GameRoot = scene;
    EditorRoot = editorScene.EditorRoot();
    return true;
}

GameObject* OpenWorld::Root() {
    return scene;
}

void OpenWorld::Close() {
    Stop();
    scene->Remove();
    editorScene.Destroy();
}

void OpenWorld::Enable() {
    /*scene->UpdateEnabled() = !IsPlaying();
    scene->RenderEnabled() = !IsPlaying();
    if (editorScene.EditorRoot()) {
        editorScene.EditorRoot()->UpdateEnabled() = !IsPlaying();
        editorScene.EditorRoot()->RenderEnabled() = !IsPlaying();
    }
    */
}

void OpenWorld::Disable() {
    /*scene->UpdateEnabled() = false;
    scene->RenderEnabled() = false;
    if (editorScene.EditorRoot()) {
        editorScene.EditorRoot()->UpdateEnabled() = false;
        editorScene.EditorRoot()->RenderEnabled() = false;
    }
    */
}

void OpenWorld::PreCompile() {
    if (runningWorld) {
        runningWorld->PreCompile();
    }
}

void OpenWorld::PostCompile() {
    if (runningWorld) {
        runningWorld->PostCompile();
    }
}

void OpenWorld::Play() {
    if (IsPlaying) return;
    
    this->GameRoot = nullptr;
    this->EditorRoot = nullptr;
        
    runningWorld = new RunningWorld();
    runningWorld->Initialize(context->Project().Path(), { scene->RootGuid() }, context->Storage());
    runningWorld->ActiveScene.Changed.Bind([this] () {
        this->GameRoot = runningWorld->ActiveScene();
        this->EditorRoot = runningWorld->EditorRoot();
    });
    Disable();
    
    IsPlaying = true;
}

void OpenWorld::Stop() {
    if (!IsPlaying) return;
    IsPlaying = false;
    GameRoot = scene;
    EditorRoot = editorScene.EditorRoot();
    runningWorld->Destroy();
    RunningWorld* delayedWorld = runningWorld;
    context->postActions.emplace_back([delayedWorld] () {
        delete delayedWorld;
    });
    runningWorld = 0;
    IsPaused = false;
    Enable();
}

void OpenWorld::Update(InputDevice& input, float dt) {
//    if (!runningWorld) return;
//    input.UpdateInputManager(&runningWorld->World().Input());
//    if (IsPaused) {
//        if (runningWorld->EditorRoot()) {
//            runningWorld->World().UpdateRoot(dt, runningWorld->EditorRoot());
//        }
//    } else {
//        runningWorld->World().Update(dt);
//    }
    //input.UpdateInputManager(&world.Input());
    //world.Update(dt);
}

void OpenWorld::Render() {
//    if (!runningWorld) return;
//    runningWorld->World().Render();
    //world.Render();
}

RunningWorld* OpenWorld::GetRunningWorld() { return runningWorld; }

GameWorld* OpenWorld::World() { return &world; }
