//
//  Project.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Project.hpp"
#include "RenderSystem.hpp"

GameWorld& Project::World() { return world; }

struct Rotator { Vector3 speed; };
struct RotatorSystem : public GameSystem<Transform, Rotator> {
    void Update(float dt) {
        for(auto o : Objects()) {
            o->GetComponent<Transform>()->EulerRotation += o->GetComponent<Rotator>()->speed * dt;
        }
    }
};

void Project::CreateDefaultScene() {

    world.CreateSystem<RenderSystem>();
    world.CreateSystem<RotatorSystem>();
    
    GameObject* camera = world.CreateObject();
    camera->AddComponent<Camera>();
    camera->AddComponent<Transform>()->Position = { 0, 0, 10 };
    camera->GetComponent<Camera>()->FieldOfView = 40;

    GameObject* cube = world.CreateObject();
    cube->AddComponent<Transform>()->Position = {0,0,0};
    cube->AddComponent<Rotator>()->speed = { 2,1,0 };
    cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
    cube->AddComponent<Material>();
    
    auto& verts = cube->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
    
    for (int i=0; i<verts.size(); i++) {
        verts[i].Color = Colour::HslToRgb(i * 10, 1, 1, 1);
    }
}
