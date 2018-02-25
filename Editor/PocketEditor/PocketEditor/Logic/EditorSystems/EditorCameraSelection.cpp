//
//  EditorCameraSelection.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 12/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "EditorCameraSelection.hpp"

void EditorCameraSelection::CreateSubSystems(GameStorage& storage) {
    storage.AddSystemType<CameraLineRenderer>();
}

void EditorCameraSelection::Initialize() {
    root->CreateSystem<CameraLineRenderer>();
}

void EditorCameraSelection::CreateObject(GameObject* object, GameObject* selectionObject) {
    selectionObject->AddComponent<EditorProxyComponent<Camera>>(object);
    selectionObject->AddComponent<Transform>(object);
    selectionObject->AddComponent<LineRenderer>();
}

void EditorCameraSelection::CameraLineRenderer::ObjectAdded(GameObject* object) {
    Camera* cam = object->GetComponent<EditorProxyComponent<Camera>>()->component;
    cam->FieldOfView.Changed.Bind(this, &EditorCameraSelection::CameraLineRenderer::CameraPropertyChanged, object);
	cam->Near.Changed.Bind(this, &EditorCameraSelection::CameraLineRenderer::CameraPropertyChanged, object);
	cam->Far.Changed.Bind(this, &EditorCameraSelection::CameraLineRenderer::CameraPropertyChanged, object);
	cam->Viewport.Changed.Bind(this, &EditorCameraSelection::CameraLineRenderer::CameraPropertyChanged, object);
	cam->Aspect.Changed.Bind(this, &EditorCameraSelection::CameraLineRenderer::CameraPropertyChanged, object);
	cam->Orthographic.Changed.Bind(this, &EditorCameraSelection::CameraLineRenderer::CameraPropertyChanged, object);
    dirtyObjects.insert(object);
}

void EditorCameraSelection::CameraLineRenderer::ObjectRemoved(GameObject* object) {
    Camera* cam = object->GetComponent<EditorProxyComponent<Camera>>()->component;
    cam->FieldOfView.Changed.Unbind(this, &EditorCameraSelection::CameraLineRenderer::CameraPropertyChanged, object);
	cam->Near.Changed.Unbind(this, &EditorCameraSelection::CameraLineRenderer::CameraPropertyChanged, object);
	cam->Far.Changed.Unbind(this, &EditorCameraSelection::CameraLineRenderer::CameraPropertyChanged, object);
	cam->Viewport.Changed.Unbind(this, &EditorCameraSelection::CameraLineRenderer::CameraPropertyChanged, object);
	cam->Aspect.Changed.Unbind(this, &EditorCameraSelection::CameraLineRenderer::CameraPropertyChanged, object);
	cam->Orthographic.Changed.Unbind(this, &EditorCameraSelection::CameraLineRenderer::CameraPropertyChanged, object);
    auto it = dirtyObjects.find(object);
    if (it!=dirtyObjects.end()) {
        dirtyObjects.erase(it);
    }
}

void EditorCameraSelection::CameraLineRenderer::CameraPropertyChanged(GameObject* object) {
    dirtyObjects.insert(object);
}

void EditorCameraSelection::CameraLineRenderer::Update(float dt) {
    for(auto o : dirtyObjects) {
        Camera* camera = o->GetComponent<EditorProxyComponent<Camera>>()->component;
        LineRenderer* lineRenderer = o->GetComponent<LineRenderer>();
        auto& v = lineRenderer->vertices;
        if (v.size()!=24) {
            v.resize(24);
        }
        
        Transform t;
        Ray ray = camera->GetRay(&t, {0,0});
        
        Vector3 vNear = ray.GetPosition(0);
        Vector3 vFar = ray.GetPosition(1);
        
        v[0] = {vNear.x, vNear.y, vNear.z };
        v[1] = {-vNear.x, vNear.y, vNear.z };
        
        v[2] = v[1];
        v[3] = {-vNear.x, -vNear.y, vNear.z };
        
        v[4] = v[3];
        v[5] = {vNear.x, -vNear.y, vNear.z };
        
        v[6] = v[5];
        v[7] = v[0];
        
        v[8] = {vFar.x, vFar.y, vFar.z };
        v[9] = {-vFar.x, vFar.y, vFar.z };
        
        v[10] = v[9];
        v[11] = {-vFar.x, -vFar.y, vFar.z };
        
        v[12] = v[11];
        v[13] = {vFar.x, -vFar.y, vFar.z };
        
        v[14] = v[13];
        v[15] = v[8];
        
        v[16] = v[0];
        v[17] = v[8];
        
        v[18] = v[2];
        v[19] = v[10];
        
        v[20] = v[4];
        v[21] = v[12];
        
        v[22] = v[6];
        v[23] = v[14];
    }
    dirtyObjects.clear();
}
