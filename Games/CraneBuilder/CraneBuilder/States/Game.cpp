//
//  Game.cpp
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/2/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "Game.h"
#include "RenderSystem.hpp"
#include "SimulationSystem.h"
#include "ParticleTransformSystem.h"
#include "ParticleFollowTransformSystem.h"
#include "TouchSystem.hpp"
#include "DraggableSystem.hpp"
#include "SpringTransformSystem.h"
#include "SpringMeshSystem.h"

void Game::Initialize() {

    Input.TouchDown += event_handler(this, &Game::MouseDown);

    world.CreateSystem<RenderSystem>();
    
    world.CreateSystem<TouchSystem>()->Input = &Input;
    world.CreateSystem<DraggableSystem>();
    world.CreateSystem<ParticleFollowTransformSystem>();
    simulation = world.CreateSystem<SimulationSystem>();
    world.CreateSystem<ParticleTransformSystem>();
    world.CreateSystem<SpringTransformSystem>();
    world.CreateSystem<SpringMeshSystem>();
    world.CreateSystem<PhysicsEditor>()->SetInputManager(Input);
    
    camera = world.CreateObject();
    camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
    camera->AddComponent<Transform>()->Position = { 0, 0, 50 };
    camera->GetComponent<Camera>()->FieldOfView = 60;
    
    //Particle* p1 = CreateBox({0,2,0}, {2,2});
    

    //CreateSpring(p1, p2, 100);
    
    /*
    
    Particle* b1;

//if (false)
{
    Particle* p1 = CreateParticle({-5,10});
    Particle* p2 = CreateParticle({-1,10});
    Particle* p3 = CreateParticle({-3,14});
    
    CreateSpring(p1, p2, 50);
    CreateSpring(p2, p3, 50);
    CreateSpring(p3, p1, 50);
    b1 = p1;
 }

//if (false)
{
    Particle* p1 = CreateParticle({0,16});
    Particle* p2 = CreateParticle({4,16});
    Particle* p3 = CreateParticle({6,20});
    
    CreateSpring(p1, p2, 50);
    CreateSpring(p2, p3, 50);
    CreateSpring(p3, p1, 50);
    spring = CreateSpring(b1, p1, 50);
 }

 CreateChain({-18,5}, {18,5}, 30, 50);
 
 CreateBox({0,20}, {3,4});
  CreateBox({-8,20}, {2,3});
  
  */
 //CreateBox({0,20}, {3,4});
  
  CreateChain({-15,-2}, {15,-2}, 50, 20);
  
  /*
  for (int y=0; y<6; y++) {
    for (int x=0; x<10; x++) {
        CreateBox({-10.0f+x*4.0f,y*4.0f}, {1.9f,1.9f});
    }
  }
  */

  
  Input.ButtonDown += event_handler(this, &Game::ButtonDown);
}

void Game::ButtonDown(std::string button) {


    if (button == "t") {
        simulation->springCollisionSystem->UseTree = !simulation->springCollisionSystem->UseTree;
    }
    
    if (button=="b") {
    Plane plane({0,0,1},0);
    
    Ray ray = camera->GetComponent<Camera>()->GetRay(camera->GetComponent<Transform>(), Input.GetTouchPosition(0));
    
    float dist;
    if (plane.IntersectsRay(ray, &dist)) {
       CreateBox(ray.GetPosition(dist), {3,3});
    }
    }
    
    if (button=="w") {
    Plane plane({0,0,1},0);
    
    Ray ray = camera->GetComponent<Camera>()->GetRay(camera->GetComponent<Transform>(), Input.GetTouchPosition(0));
    
    float dist;
    if (plane.IntersectsRay(ray, &dist)) {
       CreateWheel(ray.GetPosition(dist), 3,12);
    }
    }
    
}


Particle* Game::CreateParticle(Pocket::Vector2 p) {
    GameObject* p1 = world.CreateObject();
    p1->AddComponent<Transform>()->Position = p;
    p1->AddComponent<Mesh>()->GetMesh<Vertex>().AddGeoSphere(0, 0.5f, 6);
    p1->AddComponent<Material>();
    p1->AddComponent<Touchable>();
    p1->AddComponent<Draggable>()->Movement = Draggable::MovementMode::XYPlane;
    Particle* pa = p1->AddComponent<Particle>();
    pa->SetPosition(p);
    return pa;
}

Spring* Game::CreateSpring(Particle *p1, Particle *p2, float elasticity) {
    GameObject* go = world.CreateObject();
    go->AddComponent<Transform>();
    go->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, {4,0.3,0.01});
    go->AddComponent<Material>();
    Spring* spring = go->AddComponent<Spring>();
    spring->SetParticles(p1, p2);
    spring->elasticity = elasticity;
    return spring;
}

Particle* Game::CreateBox(Pocket::Vector2 center, Pocket::Vector2 size) {

    Vector3 corners[4] {
        center + Vector2(-size.x,-size.y),
        center + Vector2(size.x,-size.y),
        center + Vector2(size.x,size.y),
        center + Vector2(-size.x,size.y),
    };
    
    Particle* particles[4];
    
    for (int i=0; i<4; i++) {
        particles[i]=CreateParticle(corners[i]);
    }
    
    //particles[0]->mass = 10.0f;
    //particles[0]->immovable = true;
    
    float e = 70.0f;
    
    CreateSpring(particles[0], particles[1],e);
    CreateSpring(particles[1], particles[2],e);
    CreateSpring(particles[2], particles[3],e);
    CreateSpring(particles[3], particles[0],e);
    
    CreateSpring(particles[0], particles[2],e);
    //CreateSpring(particles[1], particles[3],e);
    
    return particles[0];
}

Particle* Game::CreateWheel(Vector2 center, float radius, int points) {
    std::vector<Particle*> particles;
    float deltaAngle = MathHelper::Pi * 2 / points;
    
    for (int i=0; i<points; i++) {
        float angle = deltaAngle * i;
        Vector2 offset(sinf(angle)*radius, cosf(angle)*radius);
        particles.push_back(CreateParticle(center + offset));
    }
    
    const float e = 70.0f;

    for (int i=1; i<points-1; i++) {
        CreateSpring(particles[0], particles[i],e);
        CreateSpring(particles[i], particles[i+1],e);
    }
    CreateSpring(particles[0], particles[points-1],e);
    
/*
    for (int i=0; i<points; i++) {
        CreateSpring(particles[i], particles[(i + 1) % points],e);
        CreateSpring(particles[(i + 1) % points], centerParticle,e);
        //CreateSpring(centerParticle, particles[i],e);
    }
    */
    
    //for (int i=0; i<points/2; i++) {
    //    CreateSpring(particles[i], particles[(i + points/2) % points],e);
    //}
    
    return particles[0];
}


Particle* Game::CreateChain(Pocket::Vector2 start, Pocket::Vector2 end, int links, float elasticity) {
    Vector2 delta = end-start;
    delta/=links;

    Particle* prev = 0;
    for (int i=0; i<=links; i++) {
        Vector2 position = start + delta * i;
        Particle* particle = CreateParticle(position);
        if (prev) {
            CreateSpring(prev, particle, elasticity);
        }
        prev = particle;
        particle->immovable = (i==0 || i == links);
    }
    return prev;
}

void Game::MouseDown(Pocket::TouchEvent e) {
    if (e.Index == 0) return;
    
}


void Game::Update(float dt) {
    static float time = 0;
    time+=dt*2;

    //spring->length = 5+sinf(time)*2.5f;
    world.Update(dt);
}

void Game::Render() {
    glClearColor(100.0f / 255.0f, 149.0f / 255.0f, 237.0f/255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    world.Render();
}