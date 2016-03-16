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

void DragSelector::Initialize(GameWorld* world) {
    draggingIndex = -1;
    draggableSystem = world->CreateSystem<DraggableSystem>();
    
    cameraSystem = world->CreateSystem<CameraSystem>();
    selectables = world->CreateSystem<SelectableCollection>();
}

void DragSelector::Setup(const Pocket::Box &viewport, InputManager& input) {

    BoundingBox bounds(0, Vector3(viewport.right * 2.0f, viewport.top * 2.0f, 3000.0f));
    
    renderWorld.Clear();
    renderer = renderWorld.CreateSystem<RenderSystem>();
    renderer->Octree().SetWorldBounds(bounds);
    
    
    GameObject* cameraObject = renderWorld.CreateObject();
    cameraObject->AddComponent<Transform>()->Position = Vector3(0,0,1);
    Camera* cam = cameraObject->AddComponent<Camera>();
    
    cam->Viewport = viewport;
    cam->Orthographic = true;
    cam->Near = 1.0f;
    cam->Far = 2.0f;

    dragRectangle = renderWorld.CreateObject();
    dragRectangle->AddComponent<Transform>();
    dragRectangle->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(0, 1, Colour(1.0f, 1.0f, 1.0f, 0.15f));
    dragRectangle->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
    
    input.TouchDown.Bind(this, &DragSelector::Down);
    input.TouchUp.Bind(this, &DragSelector::Up);
    
    this->input = &input;
}

void DragSelector::Down(Pocket::TouchEvent e) {
    if (draggingIndex!=-1) return;
    if (e.Index != 0) return;
    draggingIndex = e.Index;
    dragStart = e.Position;
}

void DragSelector::Up(Pocket::TouchEvent e) {
    if (draggingIndex==-1) return;
    draggingIndex = -1;
    if (draggableSystem && draggableSystem->IsTouchIndexUsed(e.Index)) return;
    SelectObjects(dragStart, e.Position);
}

bool DragSelector::IsDragging() {
    return draggingIndex!=-1;
}

void DragSelector::Update(float dt) {
    
    if (IsDragging()) {
        
        if (draggableSystem && draggableSystem->IsTouchIndexUsed(draggingIndex)) return;
        
        Vector2 dragNow = input->GetTouchPosition(draggingIndex);
        
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
    if (draggableSystem && draggableSystem->IsTouchIndexUsed(draggingIndex)) return;
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

