//
//  DragSelector.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 12/24/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "DragSelector.hpp"
#include "Camera.hpp"
#include "OctreeSystem.hpp"

using namespace Pocket;

void DragSelector::CreateSubSystems(Pocket::GameStorage &storage) {
    storage.AddSystemType<CameraSystem>();
    storage.AddSystemType<SelectableCollection<Transform>>();
}

void DragSelector::Initialize() {
    draggingIndex = -1;
    
    cameraSystem = root->CreateSystem<CameraSystem>();
    selectables = root->CreateSystem<SelectableCollection<Transform>>();
    
    root->Input().TouchDown.Bind(this, &DragSelector::Down);
    root->Input().TouchUp.Bind(this, &DragSelector::Up);
    
    TouchDepth = 0;
    dragRectangle = nullptr;
}

void DragSelector::Destroy() {
    root->Input().TouchDown.Unbind(this, &DragSelector::Down);
    root->Input().TouchUp.Unbind(this, &DragSelector::Up);
}

void DragSelector::Setup(const Pocket::Rect &viewport) {

    BoundingBox bounds(0, Vector3(viewport.width * 2.0f, viewport.height * 2.0f, 3000.0f));
    
    renderStorage.AddSystemType<RenderSystem>();
    
    renderWorld.Clear();
    renderWorld.Initialize(renderStorage);
    GameObject* root = renderWorld.CreateScene();
    renderer = root->CreateSystem<RenderSystem>();
    
    renderer->Octree().SetWorldBounds(bounds);
    
    GameObject* cameraObject = root->CreateObject();
    cameraObject->AddComponent<Transform>()->Position = Vector3(0,0,1);
    Camera* cam = cameraObject->AddComponent<Camera>();
    
    cam->Orthographic = true;
    cam->Near = 1.0f;
    cam->Far = 2.0f;
    cam->FieldOfView = 0.0f;

    dragRectangle = root->CreateObject();
    dragRectangle->AddComponent<Transform>();
    dragRectangle->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(0, 1, Colour(1.0f, 1.0f, 1.0f, 0.15f));
    dragRectangle->AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
}

void DragSelector::Down(Pocket::TouchEvent e) {
    if (draggingIndex!=-1) return;
    if (e.Index != 0) return;
    draggingIndex = e.Index;
    dragStart = e.Position;
    root->Input().SwallowTouch(e.Index, TouchDepth);
}

void DragSelector::Up(Pocket::TouchEvent e) {
    if (draggingIndex==-1) return;
    bool wasSwallowed = root->Input().IsTouchSwallowed(draggingIndex, TouchDepth);
    draggingIndex = -1;
    if (wasSwallowed) return;
    SelectObjects(dragStart, e.Position);
}

bool DragSelector::IsDragging() {
    return draggingIndex!=-1 && !root->Input().IsTouchSwallowed(draggingIndex, TouchDepth);
}

void DragSelector::Update(float dt) {
    if (!dragRectangle) return;
    
    if (IsDragging()) {
        
        Vector2 dragNow = root->Input().GetTouchPosition(draggingIndex);
        
        dragRectangle->GetComponent<Transform>()->Position = (dragStart + dragNow) * 0.5f;
        
        Vector2 delta ={dragNow.x - dragStart.x, dragNow.y - dragStart.y};
        if (delta.x<0) delta.x = -delta.x;
        if (delta.y<0) delta.y = -delta.y;
        
        dragRectangle->GetComponent<Transform>()->Scale = Vector3(delta.x, delta.y, 1);
    }
    
    renderWorld.Update(dt);
}

void DragSelector::Render() {
    if (!IsDragging()) return;
    if (!dragRectangle) return;
    renderer->Render();
}

void DragSelector::SelectObjects(Vector2 start, Vector2 end) {
    const auto& cameras = cameraSystem->Objects();
    for (auto c : cameras) {
        SelectObjectsFromCamera(c, start, end);
    }
}

void DragSelector::SelectObjectsFromCamera(GameObject* cameraObject, Vector2 start, Vector2 end) {
    Camera* camera = cameraObject->GetComponent<Camera>();
    
    Vector2 min;
    Vector2 max;
    
    if (start.x<end.x) {
        min.x = start.x;
        max.x = end.x;
    } else {
        min.x = end.x;
        max.x = start.x;
    }
    
    if (start.y<end.y) {
        min.y = start.y;
        max.y = end.y;
    } else {
        min.y = end.y;
        max.y = start.y;
    }
    
    selectables->ClearSelection();
    
    for (GameObject* go : Objects()) {
        Vector3 position = go->GetComponent<Transform>()->World().TransformPosition(go->GetComponent<Mesh>()->LocalBoundingBox().center);
        
        Vector3 screenPosition = camera->TransformPointToScreenSpace(cameraObject->GetComponent<Transform>(), position);
        go->GetComponent<Selectable>()->Selected = screenPosition.x>min.x && screenPosition.y>min.y && screenPosition.x<max.x && screenPosition.y<max.y;
    }
}

