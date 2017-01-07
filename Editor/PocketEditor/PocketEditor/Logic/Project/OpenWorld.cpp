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
#include "EditorObjectCreatorSystem.hpp"
#include "TransformHierarchy.hpp"
#include "FileHelper.hpp"
#include "InputManager.hpp"

#include "DraggableSystem.hpp"
#include "EditorTransformSystem.hpp"
#include "EditorMeshSystem.hpp"
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

OpenWorld::OpenWorld() : root(0) {
    IsPlaying.Changed.Bind([this] () {
        UpdateTimeScale();
    });
}

void OpenWorld::CreateDefaultSystems(Pocket::GameObject &world) {
    world.CreateSystem<RenderSystem>();
    world.CreateSystem<TransformHierarchy>();
    world.CreateSystem<TouchSystem>()->TouchDepth = 0;
    world.CreateSystem<ClonerSystem>();
    world.CreateSystem<TurnerSystem>();
    world.CreateSystem<EditorObjectCreatorSystem>();
    world.CreateSystem<InputMapperSystem>();
    world.CreateSystem<VelocitySystem>();
    world.CreateSystem<Gui>();
}

void OpenWorld::CreateEditorSystems(Pocket::GameObject &editorWorld) {
    editorWorld.CreateSystem<RenderSystem>();
    editorWorld.CreateSystem<TouchSystem>();
    editorWorld.CreateSystem<DraggableSystem>();
    editorWorld.CreateSystem<EditorTransformSystem>();
    editorWorld.CreateSystem<EditorMeshSystem>();
    editorWorld.CreateSystem<ClickSelectorSystem>();
    editorWorld.CreateSystem<DragSelector>()->Setup({2000,2000});
    editorWorld.CreateSystem<SelectableDragSystem>();
    editorWorld.CreateSystem<TouchSystem>()->TouchDepth = 5;
    editorWorld.CreateSystem<SelectedColorerSystem>();
    editorWorld.CreateSystem<FirstPersonMoverSystem>();
    editorWorld.CreateSystem<SelectableCollection<EditorObject>>();
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
        
        /*
        if (!root) {
            root = world->CreateRootFromJson(file, [this] (GameObject* root) {
                OpenWorld::CreateDefaultSystems(*root);
                scriptWorld->AddGameRoot(root);
                root->AddComponent<EditorObject>();
            },
            [](GameObject* go) {
                go->AddComponent<EditorObject>();
            });
        }
        file.close();
        if (!root) {
            return false;
        }
        */
    } else {
        root = world->CreateRoot();
    }
    
    CreateDefaultSystems(*root);
    scriptWorld->AddGameRoot(root);
    AddEditorObject(root);
    //auto var = root->AddComponent<CloneVariable>();
    //var->Variables.push_back({ GameIdHelper::GetClassName<Transform>(), "Rotation" });
    
    editorRoot = world->CreateRoot();
    //std::cout << "EditorRoot::scene " << editorRoot->scene<<std::endl;
    editorRoot->Order = 1;
    CreateEditorSystems(*editorRoot);
    selectables = editorRoot->CreateSystem<SelectableCollection<EditorObject>>();
    
    editorCamera = editorRoot->CreateObject();
    editorCamera->AddComponent<Camera>();
    editorCamera->AddComponent<Transform>()->Position = { 0, 0, 10 };
    editorCamera->GetComponent<Camera>()->FieldOfView = 70;
    editorCamera->AddComponent<FirstPersonMover>()->SetTouchIndices(2, 1);
    
    InitializeRoot();
    return true;
}

void OpenWorld::InitializeRoot() {
    root->CreateSystem<EditorObjectCreatorSystem>()->editorRoot = editorRoot;
    RenderSystem* worldRenderSystem = root->CreateSystem<RenderSystem>();
    RenderSystem* editorRenderSystem = editorRoot->CreateSystem<RenderSystem>();
    worldRenderSystem->SetCameras(editorRenderSystem->GetCameras());
    
    UpdateTimeScale();
}

void OpenWorld::Play() {
    if (IsPlaying) return;
    StoreWorld();
    IsPlaying = true;
}

void OpenWorld::Stop() {
    if (!IsPlaying) return;
    RestoreWorld();
    IsPlaying = false;
}

GameObject* OpenWorld::Root() {
    return root;
}

void OpenWorld::Close() {
    root->Remove();
    editorRoot->Remove();
}

void OpenWorld::Enable() {
    root->UpdateEnabled() = true;
    root->RenderEnabled() = true;
    editorRoot->UpdateEnabled() = true;
    editorRoot->RenderEnabled() = true;
}

void OpenWorld::Disable() {
    root->UpdateEnabled() = false;
    root->RenderEnabled() = false;
    editorRoot->UpdateEnabled() = false;
    editorRoot->RenderEnabled() = false;
}

void OpenWorld::UpdateTimeScale() {
    if (root) {
        root->TimeScale() = IsPlaying() ? 1.0f : 0.0f;
    }
}

void OpenWorld::AddEditorObject(Pocket::GameObject *object) {
    object->AddComponent<EditorObject>();
    for (auto child : object->Children()) {
        AddEditorObject(child);
    }
}

void OpenWorld::PreCompile() {
    Stop();
    StoreWorld();
    selectables->ClearSelection();
}

void OpenWorld::PostCompile() {
    RestoreWorld();
    Compiled();
}

void OpenWorld::StoreWorld() {
    for(auto go : selectables->Selected()) {
        EditorObject* editorObject = go->GetComponent<EditorObject>();
        storedSelectedObjects.push_back(editorObject->gameObject->RootId());
    }
    //root->ToJson(storedWorld);
    root->ToJson(storedWorld, [] (const GameObject* go, int componentID) {
        if (go->Parent() && go->Parent()->GetComponent<Cloner>()) return false;
        return true;
    });
}

void OpenWorld::RestoreWorld() {
    root->Remove();
    std::cout << storedWorld.str() << std::endl;
    root = context->World().CreateRootFromJson(storedWorld, [this] (GameObject* root) {
        CreateDefaultSystems(*root);
        context->Project().ScriptWorld().AddGameRoot(root);
    });
    storedWorld.clear();
    root->CreateSystem<EditorObjectCreatorSystem>()->editorRoot = editorRoot;
    InitializeRoot();
    std::vector<int> storedSelectedObjectsLocal = storedSelectedObjects;
    context->postActions.emplace_back([this, storedSelectedObjectsLocal] () {
        selectables->ClearSelection();
        for (auto id : storedSelectedObjectsLocal) {
            GameObject* go = root->FindObject(id);
            if (!go) continue;
            EditorObject* editorObject = go->GetComponent<EditorObject>();
            Selectable* selectable = editorObject->editorObject->GetComponent<Selectable>();
            selectable->Selected = true;
        }
    });
    storedSelectedObjects.clear();
}
