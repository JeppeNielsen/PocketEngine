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
#include <fstream>

GameWorld& OpenWorld::World() {
    return world;
}

GameWorld& OpenWorld::EditorWorld() {
    return editorWorld;
}

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

void OpenWorld::CreateDefault() {

    GameObject* gameRoot = editorWorld.CreateObject();

    RenderSystem* worldRenderSystem = world.CreateSystem<RenderSystem>();
    world.CreateSystem<TransformHierarchy>();
    world.CreateSystem<TouchSystem>()->TouchDepth = 0;
    auto creatorSystem = world.CreateSystem<EditorObjectCreatorSystem>();
    creatorSystem->editorWorld = &editorWorld;
    creatorSystem->gameRoot = gameRoot;
    world.CreateSystem<ClonerSystem>();
    world.CreateSystem<TurnerSystem>();
    
    {
        GameObject* camera = world.CreateObject();
        camera->AddComponent<Camera>();
        camera->AddComponent<Transform>()->Position = { 0, 0, 10 };
        camera->GetComponent<Camera>()->FieldOfView = 70;
    }

    RenderSystem* editorRenderSystem = editorWorld.CreateSystem<RenderSystem>();
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
    
    selectables = editorWorld.CreateSystem<SelectableCollection<EditorObject>>();
    
    {
        GameObject* camera = editorWorld.CreateObject();
        camera->AddComponent<Camera>();
        camera->AddComponent<Transform>()->Position = { 0, 0, 10 };
        camera->GetComponent<Camera>()->FieldOfView = 70;
        camera->AddComponent<FirstPersonMover>()->SetTouchIndices(2, 1);
    }
    
    worldRenderSystem->SetCameras(editorRenderSystem->GetCameras());
}

bool OpenWorld::Save() {
    bool succes = false;
    try {
        succes = true;
        std::ofstream file;
        file.open(Path);
        world.ToJson(file, [] (GameObject* go, int componentID) {
            if (componentID == Pocket::GameIDHelper::GetComponentID<EditorObject>()) return false;
            if (go->Parent() && go->Parent()()->GetComponent<Cloner>()) return false;
            return true;
        });
        file.close();
    } catch (std::exception e) {
        succes = false;
    }
    return succes;
}

bool OpenWorld::Load(const std::string &path, const std::string &filename) {
    Path = path;
    Filename = filename;
    
    CreateDefault();
    
    if (path != "") {
        std::ifstream file;
        file.open(path);
        world.CreateObject(file, 0, [](GameObject* go) {
            go->AddComponent<EditorObject>();
        });
        file.close();
    }

    return true;
}
