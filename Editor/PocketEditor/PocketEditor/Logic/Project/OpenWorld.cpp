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
#include <fstream>

GameWorld& OpenWorld::World() {
    return world;
}

GameWorld& OpenWorld::EditorWorld() {
    return editorWorld;
}

void OpenWorld::CreateDefault() {

    GameObject* gameRoot = editorWorld.CreateObject();

    world.CreateSystem<RenderSystem>();
    world.CreateSystem<TransformHierarchy>();
    world.CreateSystem<TouchSystem>()->TouchDepth = 0;
    auto creatorSystem = world.CreateSystem<EditorObjectCreatorSystem>();
    creatorSystem->editorWorld = &editorWorld;
    creatorSystem->gameRoot = gameRoot;
    world.CreateSystem<ClonerSystem>();
    
    if (false)
    {
        GameObject* camera = world.CreateObject();
        camera->AddComponent<Camera>();
        camera->AddComponent<Transform>()->Position = { 0, 0, 10 };
        camera->GetComponent<Camera>()->FieldOfView = 70;
    }

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
    
    selectables = editorWorld.CreateSystem<SelectableCollection<EditorObject>>();
    
    {
        GameObject* camera = editorWorld.CreateObject();
        camera->AddComponent<Camera>();
        camera->AddComponent<Transform>()->Position = { 0, 0, 10 };
        camera->GetComponent<Camera>()->FieldOfView = 70;
    }
}

bool OpenWorld::Save() {
    bool succes = false;
    try {
        succes = true;
        std::fstream file;
        file.open(Path);
        world.ToJson(file);
        file.close();
    } catch (std::exception e) {
      succes = false;
    }
    return succes;
}
