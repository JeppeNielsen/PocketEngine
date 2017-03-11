//
//  OpenWorld.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 13/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "OpenWorld.hpp"
#include "RenderSystem.hpp"
#include "TouchSystem.hpp"
#include "TransformHierarchy.hpp"
#include "FileHelper.hpp"
#include "InputManager.hpp"

#include "DraggableSystem.hpp"
#include "EditorTransformSystem.hpp"
#include "EditorMeshSystem.hpp"
#include "EditorSizeableSystem.hpp"
#include "ClickSelectorSystem.hpp"
#include "DragSelector.hpp"
#include "SelectableDragSystem.hpp"
#include "SelectedColorerSystem.hpp"
#include "ClonerSystem.hpp"
#include "FirstPersonMoverSystem.hpp"
#include "ScriptWorld.hpp"
#include "InputMapperSystem.hpp"
#include <fstream>
#include "Project.hpp"
#include "CloneVariable.hpp"
#include "VelocitySystem.hpp"
#include "EditorContext.hpp"
#include "AssetManager.hpp"
#include "SwitchSystem.hpp"
#include "TouchSwitchSystem.hpp"
#include "SwitchEnablerSystem.hpp"
#include "SlicedQuadMeshSystem.hpp"

#include "TriggerSystem.hpp"
#include "TriggerTouchSystem.hpp"
#include "SceneManagerSystem.hpp"

struct Turner {
    Vector3 speed;
    
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(speed)
    TYPE_FIELDS_END
};

struct TurnerSystem : public GameSystem<Transform, Turner> {
    void Update(float dt) {
        for(auto o : Objects()) {
            o->GetComponent<Transform>()->EulerRotation += o->GetComponent<Turner>()->speed * dt;
        }
    }
};

OpenWorld::OpenWorld() : root(0), runningWorld(0) {
    IsPlaying = false;
    EditorRoot = 0;
}

void OpenWorld::CreateDefaultSystems(Pocket::GameObject &world) {
    world.CreateSystem<RenderSystem>();
    world.CreateSystem<TransformHierarchy>();
    world.CreateSystem<TouchSystem>()->TouchDepth = 0;
    world.CreateSystem<ClonerSystem>();
    world.CreateSystem<TurnerSystem>();
    world.CreateSystem<InputMapperSystem>();
    world.CreateSystem<VelocitySystem>();
    world.CreateSystem<Gui>();
    world.CreateSystem<SwitchSystem>();
    world.CreateSystem<SwitchEnablerSystem>();
    world.CreateSystem<TouchSwitchSystem>();
    world.CreateSystem<SlicedQuadMeshSystem>();
    world.CreateSystem<TriggerSystem>();
    world.CreateSystem<TriggerTouchSystem>();
    world.CreateSystem<SceneManagerSystem>();
}

void OpenWorld::CreateEditorSystems(Pocket::GameObject &editorWorld) {
    editorWorld.CreateSystem<RenderSystem>();
    editorWorld.CreateSystem<TouchSystem>();
    editorWorld.CreateSystem<DraggableSystem>();
    editorWorld.CreateSystem<EditorTransformSystem>();
    editorWorld.CreateSystem<EditorMeshSystem>();
    editorWorld.CreateSystem<EditorSizeableSystem>();
    
    editorWorld.CreateSystem<TransformHierarchy>();
    
    editorWorld.CreateSystem<ClickSelectorSystem>();
    editorWorld.CreateSystem<DragSelector>()->Setup({2000,2000});
    editorWorld.CreateSystem<SelectableDragSystem>();
    editorWorld.CreateSystem<TouchSystem>()->TouchDepth = 5;
    editorWorld.CreateSystem<SelectedColorerSystem>();
    editorWorld.CreateSystem<FirstPersonMoverSystem>();
    editorWorld.CreateSystem<SelectableCollection<EditorObject>>();
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

    CreateDefaultSystems(*root);
    scriptWorld->AddGameRoot(root);
    
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
    editorScene.EditorRoot()->UpdateEnabled() = !IsPlaying();
    editorScene.EditorRoot()->RenderEnabled() = !IsPlaying();
}

void OpenWorld::Disable() {
    root->UpdateEnabled() = false;
    root->RenderEnabled() = false;
    editorScene.EditorRoot()->UpdateEnabled() = false;
    editorScene.EditorRoot()->RenderEnabled() = false;
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
    Stop();
}

void OpenWorld::PostCompile() {
    Compiled();
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
