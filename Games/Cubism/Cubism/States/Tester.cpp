//
//  Tester.cpp
//  Cubism
//
//  Created by Jeppe Nielsen on 01/12/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "Tester.hpp"
#include "RenderSystem.hpp"

Component(Spinner)
    public:
        Vector3 amount;
};

SYSTEM(SpinnerSystem, Transform, Spinner)
        void Update(float dt) {
            for (auto o : Objects()) {
                auto q = o->GetComponent<Transform>()->Rotation;
                q*=Quaternion(o->GetComponent<Spinner>()->amount*dt);
                o->GetComponent<Transform>()->Rotation = q;
            }
        }
};

void Tester::Initialize() {
    
    RenderSystem* renderSystem = world.CreateSystem<RenderSystem>();
    world.CreateSystem<SpinnerSystem>();
    
    GameObject* camera = world.CreateObject();
    camera->AddComponent<Transform>()->Position = {0,0,5};
    camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
    
    GameObject* cube = world.CreateObject();
    cube->AddComponent<Transform>()->Position = {0,0,0};
    cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
    cube->AddComponent<Material>()->Shader = &renderSystem->Shaders.Colored;
    cube->AddComponent<Spinner>()->amount = {0,2,3};
    
}

void Tester::Update(float dt) {
    world.Update(dt);
}

void Tester::Render() {
    glClearColor(1, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    world.Render();
}