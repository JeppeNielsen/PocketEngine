//
//  BackgroundSystem.cpp
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 08/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "BackgroundSystem.h"

void BackgroundSystem::ObjectAdded(Pocket::GameObject *object) {
    Background* background = object->GetComponent<Background>();
    auto& mesh = object->GetComponent<Mesh>()->GetMesh<Vertex>();
    
    const Vector2 size(50,50);
    
    mesh.vertices.resize(4);
    
    mesh.vertices[0].Position = {-size.x, -size.y,0};
    mesh.vertices[1].Position = {-size.x, size.y,0};
    mesh.vertices[2].Position = {size.x, size.y,0};
    mesh.vertices[3].Position = {size.x, -size.y,0};
    
    mesh.triangles.resize(6);
    mesh.triangles[0]=0;
    mesh.triangles[1]=2;
    mesh.triangles[2]=1;
    
    mesh.triangles[3]=0;
    mesh.triangles[4]=3;
    mesh.triangles[5]=2;
    
    for(int i=0; i<4; i++) {
        mesh.vertices[i].Color = background->colors[i];
    }
}

void BackgroundSystem::ObjectRemoved(Pocket::GameObject *object) {

}

void BackgroundSystem::Update(float dt){
    Camera* camera = CameraObject->GetComponent<Camera>();
    Transform* transform = CameraObject->GetComponent<Transform>();
    
    Vector3 bottomLeft = camera->TransformViewportToWorld(transform, {-1,-1,0.9999f});
    Vector3 topRight = camera->TransformViewportToWorld(transform, {1,1,0.9999f});
    
    Ray ray = camera->GetRay(transform, {0,0});
    
    for(GameObject* go : Objects()) {
        auto& mesh = go->GetComponent<Mesh>()->GetMesh<Vertex>();
        mesh.vertices[0].Position = {bottomLeft.x, bottomLeft.y,bottomLeft.z};
        mesh.vertices[1].Position = {bottomLeft.x, topRight.y,bottomLeft.z};
        mesh.vertices[2].Position = {topRight.x, topRight.y,bottomLeft.z};
        mesh.vertices[3].Position = {topRight.x, bottomLeft.y,bottomLeft.z};
    }
}