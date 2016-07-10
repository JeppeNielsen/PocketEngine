//
//  EditorSpawnerSystem.cpp
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 09/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "EditorSpawnerSystem.h"
#include "Transform.hpp"

void EditorSpawnerSystem::ObjectAdded(Pocket::GameObject *object) {
    auto& mesh = object->GetComponent<Mesh>()->GetMesh<Vertex>();
    const Vector2 size(3000,3000);
    
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

    object->GetComponent<Touchable>()->Click .Bind(this, &EditorSpawnerSystem::Click);
}

void EditorSpawnerSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Click .Unbind(this, &EditorSpawnerSystem::Click);
}

void EditorSpawnerSystem::Click(Pocket::TouchData d) {
    EditorSpawner* spawner = d.object->GetComponent<EditorSpawner>();
    auto& factoryFunction = spawner->objects[state->CurrentState()][spawner->CurrentObject];
    if (!factoryFunction) return;
    
    GameObject* clone = factoryFunction(d.WorldPosition);
    Transform* transform = clone->GetComponent<Transform>();
    if (transform) {
        Vector3 position = d.WorldPosition;
        position.z = 0;
        transform->Position = position;
    }
}