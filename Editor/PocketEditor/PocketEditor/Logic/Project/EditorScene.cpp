//
//  EditorWorld.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 03/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "EditorScene.hpp"


#include "RenderSystem.hpp"
#include "TouchSystem.hpp"
#include "TransformHierarchy.hpp"

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


void EditorScene::CreateEditorSystems(Pocket::GameObject &editorWorld) {
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

bool EditorScene::IsClonerInAncestry(GameObject* object) {
    while(true) {
        object = object->Parent;
        if (!object) return false;
        if (object->HasComponent<Cloner>()) return true;
    }
}

void EditorScene::BindToRoot(Pocket::GameObject *root) {
    
    root->SetCallbacks(
        [this] (GameObject* object) {
            if (IsClonerInAncestry(object)) return;
            AddObjectToEditor(object);
        }
        ,
        [this] (GameObject* object) {
            auto it = rootToEditorMap.find(object);
            if (it == rootToEditorMap.end()) return;
            GameObject* editorGameObject = it->second;
            editorGameObject->Remove();
            rootToEditorMap.erase(rootToEditorMap.find(object));
        }
        ,
        [this] (GameObject* object, ComponentId componentId) {
             auto it = rootToEditorMap.find(object);
            if (it == rootToEditorMap.end()) return;
            GameObject* editorGameObject = it->second;
            for(auto id : editorObjectsComponents) {
                if (id == componentId) {
                    editorGameObject->AddComponent(id, object);
                    break;
                }
            }
        }
        ,
        [this] (GameObject* object, ComponentId componentId) {
            auto it = rootToEditorMap.find(object);
            if (it == rootToEditorMap.end()) return;
            GameObject* editorGameObject = it->second;
            for(auto id : editorObjectsComponents) {
                if (id == componentId) {
                    editorGameObject->RemoveComponent(id);
                    break;
                }
            }
        }
    );
}

void EditorScene::AddEditorObject(Pocket::GameObject *object) {
    
    auto editorGameObject = AddObjectToEditor(object);
    
    for(auto id : editorObjectsComponents) {
        if (object->HasComponent(id)) {
            editorGameObject->AddComponent(id, object);
        }
    }
    for (auto child : object->Children()) {
        AddEditorObject(child);
    }
}

GameObject* EditorScene::AddObjectToEditor(GameObject* object) {

    GameObject* editorGameObject = editorRoot->CreateObject();
    auto editorObject = object->AddComponent<EditorObject>();
    editorObject->editorObject = editorGameObject;
    editorObject->gameObject = object;
    
    editorGameObject->AddComponent<EditorObject>(object);
    editorGameObject->AddComponent<Selectable>();
    
    rootToEditorMap[object] = editorGameObject;
    
    return editorGameObject;
}

EditorScene::EditorScene() : sceneRoot(0), editorRoot(0) {

}

void EditorScene::Initialize(Pocket::GameObject *sceneRoot) {
    if (this->sceneRoot) {
        Destroy();
    }
    this->sceneRoot = sceneRoot;
    editorObjectsComponents = {
        GameIdHelper::GetComponentID<Transform>(),
        GameIdHelper::GetComponentID<Mesh>(),
        GameIdHelper::GetComponentID<Sizeable>()
    };
    editorRoot = sceneRoot->World()->CreateRoot();
    CreateEditorSystems(*editorRoot);
    
    AddEditorObject(sceneRoot);
    
    GameObject* editorCamera = editorRoot->CreateObject();
    editorCamera->AddComponent<Camera>();
    editorCamera->AddComponent<Transform>()->Position = { 0, 0, 3 };
    editorCamera->GetComponent<Camera>()->FieldOfView = 70;
    editorCamera->AddComponent<FirstPersonMover>()->SetTouchIndices(2, 1);
    
    editorRoot->Order = 100;
    sceneRoot->CreateSystem<RenderSystem>()->SetCameras(editorRoot->CreateSystem<RenderSystem>()->GetCameras());
    
    BindToRoot(sceneRoot);
}

void EditorScene::Destroy() {
    if (!sceneRoot) return;
    
    for(auto o : rootToEditorMap) {
        o.first->RemoveComponent<EditorObject>();
    }
    rootToEditorMap.clear();
    
    sceneRoot->SetCallbacks(0, 0, 0, 0);
    editorRoot->Remove();
    sceneRoot->CreateSystem<RenderSystem>()->SetCameras(sceneRoot->CreateSystem<RenderSystem>()->GetOriginalCameras());
    sceneRoot = 0;
    editorRoot = 0;
}

GameObject* EditorScene::GameRoot() {
    return sceneRoot;
}

GameObject* EditorScene::EditorRoot() {
    return editorRoot;
}
