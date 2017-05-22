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

OpenWorld::OpenWorld() : root(0), runningWorld(0) {
    IsPlaying = false;
    EditorRoot = 0;
}

bool IsClonerInAncestry(GameObject* object) {
    while(true) {
        object = object->Parent;
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
        root->ToJson(file, [] (const GameObject* go, int componentID) {
            if (componentID == Pocket::GameIdHelper::GetComponentID<EditorObject>()) return false;
            if (go->Parent() && go->Parent()->GetComponent<Cloner>()) return false;
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
    
    GameWorld* world = &context->World();
    ScriptWorld* scriptWorld = &context->Project().ScriptWorld();
    
    if (path != "") {
        std::ifstream file;
        
        file.open(path);
        std::string guid = world->ReadGuidFromJson(file);
        file.close();
        
        root = world->TryFindRoot(guid);
        if (!root) {
            return false;
        }
    } else {
        return false;
    }
    
    root->TimeScale() = 0;

    SystemHelper::AddGameSystems(*root);
    
    if (context->Project().IsCompiling) {
        scriptWorld->AddGameRoot(root);
    }
    
    editorScene.Initialize(root);
    
    GameRoot = root;
    EditorRoot = editorScene.EditorRoot();
    return true;
}

GameObject* OpenWorld::Root() {
    return root;
}

void OpenWorld::Close() {
    Stop();
    root->Remove();
    editorScene.Destroy();
}

void OpenWorld::Enable() {
    root->UpdateEnabled() = !IsPlaying();
    root->RenderEnabled() = !IsPlaying();
    if (editorScene.EditorRoot()) {
        editorScene.EditorRoot()->UpdateEnabled() = !IsPlaying();
        editorScene.EditorRoot()->RenderEnabled() = !IsPlaying();
    }
}

void OpenWorld::Disable() {
    root->UpdateEnabled() = false;
    root->RenderEnabled() = false;
    if (editorScene.EditorRoot()) {
        editorScene.EditorRoot()->UpdateEnabled() = false;
        editorScene.EditorRoot()->RenderEnabled() = false;
    }
}

/*
void OpenWorld::UpdatePlayMode() {
    if (root) {
        
        RenderSystem* worldRenderSystem = root->CreateSystem<RenderSystem>();
        RenderSystem* editorRenderSystem = editorRoot->CreateSystem<RenderSystem>();
        
        TouchSystem* worldTouchSystem = root->CreateSystem<TouchSystem>();
        TouchSystem* editorTouchSystem = editorRoot->CreateSystem<TouchSystem>();
        
        if (!IsPlaying || worldRenderSystem->GetOriginalCameras()->ObjectCount() == 0) {
            editorTouchSystem->SetCameras(editorTouchSystem->GetOriginalCameras());
            editorRenderSystem->SetCameras(editorRenderSystem->GetOriginalCameras());
            
            worldTouchSystem->SetCameras(editorTouchSystem->GetOriginalCameras());
            worldRenderSystem->SetCameras(editorRenderSystem->GetOriginalCameras());
            
            editorRoot->RenderEnabled() = true;
        } else {
        
            editorTouchSystem->SetCameras(worldTouchSystem->GetOriginalCameras());
            editorRenderSystem->SetCameras(worldRenderSystem->GetOriginalCameras());
            
            worldTouchSystem->SetCameras(worldTouchSystem->GetOriginalCameras());
            worldRenderSystem->SetCameras(worldRenderSystem->GetOriginalCameras());
            
            editorRoot->RenderEnabled() = false;
        }

        root->TimeScale() = IsPlaying ? 1.0f : 0.0f;
    }
}
*/

void OpenWorld::PreCompile() {
    if (runningWorld) {
        runningWorld->PreCompile();
    }
}

void OpenWorld::PostCompile() {

    context->Project().ScriptWorld().AddGameRoot(root);

    if (runningWorld) {
        runningWorld->PostCompile();
    }
}

void OpenWorld::Play() {
    if (IsPlaying) return;
    
    this->GameRoot = 0;
    this->EditorRoot = 0;
        
    runningWorld = new RunningWorld();
    runningWorld->Initialize(context->Project().Path(), { root->RootGuid() }, context->Project().ScriptWorld());
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
    GameRoot = root;
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
    if (!runningWorld) return;
    input.UpdateInputManager(&runningWorld->World().Input());
    if (IsPaused) {
        if (runningWorld->EditorRoot()) {
            runningWorld->World().UpdateRoot(dt, runningWorld->EditorRoot());
        }
    } else {
        runningWorld->World().Update(dt);
    }
}

void OpenWorld::Render() {
    if (!runningWorld) return;
    runningWorld->World().Render();
}

RunningWorld* OpenWorld::GetRunningWorld() { return runningWorld; }
