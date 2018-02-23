//
//  EditorWorld.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 03/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "EditorScene.hpp"
#include "RenderSystem.hpp"
#include "SystemHelper.hpp"
#include "EditorObject.hpp"
#include "Cloner.hpp"
#include "Selectable.hpp"
#include "Mesh.hpp"
#include "FirstPersonMover.hpp"
#include "EditorProxyComponent.hpp"
#include "Sizeable.hpp"
#include "DragSelector.hpp"
#include "TouchSystem.hpp"

bool EditorScene::IsClonerInAncestry(GameObject* object) {
    while(true) {
        object = object->Hierarchy().Parent;
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
            
            auto proxyIt = proxyComponents.find(componentId);
            
            if (proxyIt==proxyComponents.end()) {
                for(auto id : editorObjectsComponents) {
                    if (id == componentId) {
                        editorGameObject->AddComponent(id, object);
                        break;
                    }
                }
            } else {
                proxyIt->second.add(object, editorGameObject);
            }
        }
        ,
        [this] (GameObject* object, ComponentId componentId) {
            auto it = rootToEditorMap.find(object);
            if (it == rootToEditorMap.end()) return;
            GameObject* editorGameObject = it->second;
            
            auto proxyIt = proxyComponents.find(componentId);
            
            if (proxyIt==proxyComponents.end()) {
                for(auto id : editorObjectsComponents) {
                    if (id == componentId) {
                        editorGameObject->RemoveComponent(id);
                        break;
                    }
                }
            } else {
                proxyIt->second.remove(editorGameObject);
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
    
    for(auto proxyIt : proxyComponents) {
        if (object->HasComponent(proxyIt.first)) {
            proxyIt.second.add(object, editorGameObject);
        }
    }
    for (auto child : object->Hierarchy().Children()) {
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
        GameIdHelper::GetComponentID<Sizeable>(),
    };
    
    proxyComponents[GameIdHelper::GetComponentID<Camera>()] = {
        [] (GameObject* o, GameObject* eo) {
            eo->AddComponent<EditorProxyComponent<Camera>>()->component = o->GetComponent<Camera>();
        },
        [] (GameObject* o) {
            o->RemoveComponent<EditorProxyComponent<Camera>>();
        }
    };
    
    editorRoot = sceneRoot->World()->CreateScene();
    SystemHelper::AddEditorSystems(*editorRoot);
    
    AddEditorObject(sceneRoot);
    
    GameObject* editorCamera = editorRoot->CreateObject();
    editorCamera->AddComponent<Camera>();
    editorCamera->AddComponent<Transform>()->Position = { 0, 0, 30 };
    editorCamera->GetComponent<Camera>()->FieldOfView = 70;
    editorCamera->AddComponent<FirstPersonMover>()->SetTouchIndices(2, 1);
    
    editorRoot->Hierarchy().Order = 100;
    sceneRoot->CreateSystem<RenderSystem>()->SetCameras(editorRoot->CreateSystem<RenderSystem>()->GetCameras());
    editorRoot->CreateSystem<DragSelector>()->Setup({0,0,2000,2000});
    editorRoot->CreateSystem<DragSelector>()->TouchDepth = 50;
    editorRoot->CreateSystem<TouchSystem>()->TouchDepth = 100;
    
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
