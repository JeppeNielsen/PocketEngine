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
#include "Orderable.hpp"
#include "BeamMeshSystem.h"
#include "HydralicMeshSystem.h"
#include "ParticleMeshSystem.h"
#include "HydralicSystem.h"
#include "TerrainMeshSystem.h"

void Game::Initialize() {

    renderSystem = world.CreateSystem<RenderSystem>();
    
    world.CreateSystem<TouchSystem>()->Input = &Input;
    creatorSystem = world.CreateSystem<CreatorSystem>();
    
    world.CreateSystem<DraggableSystem>();
    world.CreateSystem<ParticleFollowTransformSystem>();
    simulation = world.CreateSystem<SimulationSystem>();
    world.CreateSystem<ParticleTransformSystem>();
    world.CreateSystem<SpringTransformSystem>();
    world.CreateSystem<BeamMeshSystem>();
    world.CreateSystem<HydralicMeshSystem>();
    world.CreateSystem<ParticleMeshSystem>();
    world.CreateSystem<HydralicSystem>();
    world.CreateSystem<TerrainMeshSystem>();
    
    camera = world.CreateObject();
    camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
    camera->AddComponent<Transform>()->Position = { 0, -15, 80 };
    camera->GetComponent<Camera>()->FieldOfView = 60;
    
    atlas = world.CreateObject();
    Texture& texture = atlas->AddComponent<TextureComponent>()->Texture();
    texture.LoadFromPng("images.png");
    atlas->AddComponent<class Atlas>()->Load("images.xml",Vector2(texture.GetWidth(), texture.GetHeight()));
    
    buildType = BuildType::Beam;
    
    creator = world.CreateObject();
    creator->AddComponent<Creator>();
    
    currentHydralic = 0;
    
    CreateParticle({-5,0})->GetComponent<Particle>()->immovable = true;
    CreateParticle({5,0})->GetComponent<Particle>()->immovable = true;
    
    
    creatorSystem->CreateParticle = [this] () -> GameObject* {
        return CreateParticle(0);
    };

    creatorSystem->CreateSpring = [this] () -> GameObject* {
        GameObject* s = CreateSpring(70.0f);
        if (buildType == BuildType::Beam) {
            s->AddComponent<Beam>();
        } else if (buildType == BuildType::Hydralic) {
            currentHydralic = s->AddComponent<Hydralic>();
        }
        return s;
    };
    
    creatorSystem->Continuous = [this] () -> bool {
        return buildType == BuildType::Beam;
    };
    
    GameObject* t = world.CreateObject();
    Terrain* terrain = t->AddComponent<Terrain>();
    
    //terrain->vertices = {{40,0}, {0,20}, {-20,5}, {-40,0}, {-40,-20}, {40,-20}};
    //terrain->vertices = {{40,0}, {-40,0}, {-40,-20}, {40,-20}};
    
    terrain->vertices = {
        {-50,0},{-50,-10},{-20,-10},{-20,-50},{-50,-50}, {-50,-60},{50,-60},{50,-50},{20,-50},{20,-10},{50,-10},{50,0},
    };
    
    
    /*
    int segments = 128;
    float dAngle = -MathHelper::DegToRad * 360.0f / segments;
    
    for (int s=0; s<segments; s++) {
        terrain->vertices.push_back({ sinf(s*dAngle)*40, cosf(s*dAngle)*5 });
    }
    */
    
    
    Terrain::Layer grass;
    grass.outerDepth = -3;
    grass.innerDepth = 0;
    grass.textureScale  = 0.06f;
    grass.outerV = 8.0f / 256.0f;
    grass.innerV = 67.0f / 256.0f;
    grass.isBevel = false;
    grass.useDirection = true;
    grass.direction = {0,1};
    grass.directionAmount = 0;
    
    terrain->layers.push_back(grass);
    
    /*
    Terrain::Layer dirt;
    dirt.outerDepth = -1;
    dirt.innerDepth = 0.4f;
    dirt.textureScale  = -0.03f;
    dirt.outerV = 217.0f / 256.0f;
    dirt.innerV = 249.0f / 256.0f;
    dirt.isBevel = false;
    dirt.useDirection = true;
    dirt.direction = {1,0};
    dirt.directionAmount = 0.9f;
    
    terrain->layers.push_back(dirt);
    
    Terrain::Layer dirt2;
    dirt2.outerDepth = -1;
    dirt2.innerDepth = 0.4f;
    dirt2.textureScale  = 0.03f;
    dirt2.outerV = 217.0f / 256.0f;
    dirt2.innerV = 249.0f / 256.0f;
    dirt2.isBevel = false;
    dirt2.useDirection = true;
    dirt2.direction = {-1,0};
    dirt2.directionAmount = 0.9f;
    
    terrain->layers.push_back(dirt2);
    
    */
    
    
    
    Terrain::Layer shadows;
    shadows.outerDepth = -1;
    shadows.innerDepth = 0.25f;
    shadows.textureScale  = 0.06f;
    shadows.outerV = 69.0f / 256.0f;
    shadows.innerV = 129.0f / 256.0f;
    shadows.isBevel = false;
    shadows.useDirection = true;
    shadows.direction = {0,-1};
    shadows.directionAmount = 0.4f;
    shadows.color = Colour::White(0.5f);
    
    terrain->layers.push_back(shadows);
    
    Terrain::Layer outline;
    outline.outerDepth = -0.35f;
    outline.innerDepth = 0.35f;
    outline.textureScale  = 0.1f;
    outline.outerV = 69.0f / 256.0f;
    outline.innerV = 118.0f / 256.0f;
    outline.isBevel = false;
    
    terrain->layers.push_back(outline);
    
    Terrain::Layer bevel;
    bevel.outerDepth = 0.3f;
    bevel.innerDepth = 5;
    bevel.textureScale  = 0.1f;
    bevel.outerV = 2.0f / 256.0f;
    bevel.innerV = bevel.outerV;
    bevel.isBevel = true;
    
    terrain->layers.push_back(bevel);
    
    
    
    
    
    
    t->AddComponent<Transform>();
    t->AddComponent<Mesh>();
    t->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
    t->AddComponent<TextureComponent>()->Texture().LoadFromPng("Terrain.png");
    t->AddComponent<Orderable>()->Order = -50;
    
    
    GameObject* center = world.CreateObject();
    center->Parent = t;
    center->AddComponent<Transform>();
    center->AddComponent<Mesh>();
    center->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
    center->AddComponent<TextureComponent>()->Texture().LoadFromPng("Dirt.png");
    center->AddComponent<Orderable>()->Order = -100;
    
    GameObject* bevelObject = world.CreateObject();
    bevelObject->Parent = t;
    bevelObject->AddComponent<Transform>();
    bevelObject->AddComponent<Mesh>();
    bevelObject->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
    bevelObject->AddComponent<TextureComponent>(t);
    bevelObject->AddComponent<Orderable>()->Order = -25;
    
    GameObject* platformObject = world.CreateObject();
    platformObject->Parent = t;
    platformObject->AddComponent<Transform>();
    platformObject->AddComponent<Mesh>();
    platformObject->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
    platformObject->AddComponent<Orderable>()->Order = -20;
    platformObject->AddComponent<TextureComponent>()->Texture().LoadFromPng("Platform.png");

  
    Input.ButtonDown += event_handler(this, &Game::ButtonDown);
    Input.ButtonUp += event_handler(this, &Game::ButtonUp);
    wireframe = false;
}

void Game::ButtonUp(std::string button) {
    if (button == "" && currentHydralic) {
        currentHydralic->speed = 0.0f;
    }
    if (button == "" && currentHydralic) {
        currentHydralic->speed = 0.0f;
    }
}

void Game::ButtonDown(std::string button) {

    if (button == " ") {
        creatorSystem->Cancel();
        simulation->Running = !simulation->Running();
    }

    if (button == "1") buildType = BuildType::Beam;
    if (button == "2") buildType = BuildType::Hydralic;
    
    if (button == "" && currentHydralic) {
        currentHydralic->speed = 3.0f;
    }
    
    if (button == "" && currentHydralic) {
        currentHydralic->speed = -3.0f;
    }
    
    if (button == "w") wireframe =!wireframe;

/*
    if (button == "t") {
        simulation->springCollisionSystem->UseTree = !simulation->springCollisionSystem->UseTree;
    }
    
    if (button=="b") {
    Plane plane({0,0,1},0);
    
    Ray ray = camera->GetComponent<Camera>()->GetRay(camera->GetComponent<Transform>(), Input.GetTouchPosition(0));
    
    float dist;
    if (plane.IntersectsRay(ray, &dist)) {
       CreateBox(ray.GetPosition(dist), {3,3});
       //Vector2 position = ray.GetPosition(dist);
       //CreateParticle(position);
       
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
    */
}


GameObject* Game::CreateParticle(Vector2 p) {
    GameObject* p1 = world.CreateObject();
    p1->AddComponent<Transform>()->Position = p;
    p1->AddComponent<Mesh>();
    p1->AddComponent<Atlas>(atlas);
    p1->AddComponent<TextureComponent>(atlas);
    p1->AddComponent<Material>();//->Shader = &renderSystem->Shaders.Textured;
    p1->GetComponent<Material>()->BlendMode = BlendModeType::Alpha;
    p1->AddComponent<Touchable>();
    p1->AddComponent<Creator>(creator);
    
    static int counter = 10000;
    counter++;
    
    p1->AddComponent<Orderable>()->Order = counter;
    p1->AddComponent<Draggable>()->Movement = Draggable::MovementMode::XYPlane;
    Particle* pa = p1->AddComponent<Particle>();
    pa->SetPosition(p);
    return p1;
}

GameObject* Game::CreateSpring(float elasticity) {
    GameObject* go = world.CreateObject();
    go->AddComponent<Transform>();
    go->AddComponent<Mesh>();
    go->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
    go->AddComponent<Atlas>(atlas);
    go->AddComponent<TextureComponent>(atlas);
    Spring* spring = go->AddComponent<Spring>();
    spring->elasticity = elasticity;
    
    static int counter = 0;
    counter++;
    go->AddComponent<Orderable>()->Order = counter;
    return go;
}

/*
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
*/

void Game::Update(float dt) {
    world.Update(dt);
}

void Game::Render() {
    if (wireframe) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        } else {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
    glClearColor(100.0f / 255.0f, 149.0f / 255.0f, 237.0f/255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    world.Render();
}