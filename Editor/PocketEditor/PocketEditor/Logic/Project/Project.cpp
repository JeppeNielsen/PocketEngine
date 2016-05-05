//
//  Project.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Project.hpp"
#include "RenderSystem.hpp"
#include "EditorObjectCreatorSystem.hpp"
#include "TransformHierarchy.hpp"

GameWorld& Project::World() { return world; }



void Project::CreateDefaultScene(GameWorld& editorWorld, GameObject* gameRoot) {

    world.CreateSystem<RenderSystem>();
    world.CreateSystem<RotatorSystem>();
    world.CreateSystem<TransformHierarchy>();
    auto creatorSystem = world.CreateSystem<EditorObjectCreatorSystem>();
    creatorSystem->editorWorld = &editorWorld;
    creatorSystem->gameRoot = gameRoot;
    
    GameObject* camera = world.CreateObject();
    camera->AddComponent<Camera>();
    camera->AddComponent<Transform>()->Position = { 0, 0, 10 };
    camera->GetComponent<Camera>()->FieldOfView = 70;


    for (int x=-1; x<=1; ++x) {
    for (int y=-1; y<=1; ++y) {
    
        GameObject* cube = world.CreateObject();
        cube->AddComponent<Transform>()->Position = {x*2.3f,y*2.3f,0};
        cube->AddComponent<Rotator>()->speed = { 0,0.3f,0 };
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 0.2f);
        cube->AddComponent<Material>();
        cube->AddComponent<EditorObject>();
        
        auto& verts = cube->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
        
        for (int i=0; i<verts.size(); i++) {
            verts[i].Color = Colour::HslToRgb(i * 10, 1, 1, 1);
        }
        
        GameObject* child = world.CreateObject();
        child->Parent = cube;
        child->AddComponent<Transform>()->Position = {0,0,-2};
        child->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 0.5f);
        child->AddComponent<Material>();
        child->AddComponent<EditorObject>();
        }
    }
    
    
}
