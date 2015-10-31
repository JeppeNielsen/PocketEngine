//
//  BuilderEditor.cpp
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/19/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "PhysicsEditor.h"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Plane.hpp"
#include "Camera.hpp"
#include "Spring.h"
#include <iostream>
#include "Draggable.hpp"

void PhysicsEditor::Initialize() {
    AddComponent<Particle>();
    AddComponent<Touchable>();
    modifingParticle = 0;
    createdParticle = 0;
    
    touchUp = false;
    particleClicked = false;
}

void PhysicsEditor::SetInputManager(Pocket::InputManager &input) {
    this->input = &input;
    input.TouchUp += event_handler(this, &PhysicsEditor::TouchUp);
}

void PhysicsEditor::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Click += event_handler(this, &PhysicsEditor::ParticleClicked, object);
}

void PhysicsEditor::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Click -= event_handler(this, &PhysicsEditor::ParticleClicked, object);
}

void PhysicsEditor::ParticleClicked(Pocket::TouchData d, Pocket::GameObject *object) {
    if (modifingParticle == object) {
        modifingParticle = 0;
        if (createdParticle) {
            createdParticle->Remove();
            createdParticle = 0;
        }
        return;
    }

    if (!modifingParticle) {
        modifingParticle = object;
        
        if (!createdParticle) {
            createdParticle = CreateParticle();
        }

        cameraObject = d.CameraObject;
    } else {
        
        Spring* spring = CreateSpring();
        spring->SetParticles(modifingParticle->GetComponent<Particle>(), object->GetComponent<Particle>());
        modifingParticle = object;
    }
    particleClicked = true;
}

Spring* PhysicsEditor::CreateSpring() {
    GameObject* go = World()->CreateObject();
    go->AddComponent<Transform>();
    go->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, {4,0.3,0.01});
    go->AddComponent<Material>();
    Spring* spring = go->AddComponent<Spring>();
    spring->elasticity = 50.0f;
    return spring;
}

GameObject* PhysicsEditor::CreateParticle() {
    GameObject* p = World()->CreateObject();
    p->AddComponent<Particle>()->immovable = true;
    p->AddComponent<Transform>();
    p->AddComponent<Mesh>()->GetMesh<Vertex>().AddGeoSphere(0, 0.5f, 6);
    p->AddComponent<Material>();
    p->AddComponent<Draggable>()->Movement = Draggable::MovementMode::XYPlane;
    return p;
}

void PhysicsEditor::TouchUp(Pocket::TouchEvent e) {
    if (!modifingParticle) return;
    if (e.Index == 1) {
        
        modifingParticle = 0;
        if (createdParticle) {
            createdParticle->Remove();
            createdParticle = 0;
        }
    
        return;
    }
    touchUp = true;
}
/*
void Dummy() {

    Plane plane({0,0,1},0);
    
    Ray ray = cameraObject->GetComponent<Camera>()->GetRay(cameraObject->GetComponent<Transform>(), Input->GetTouchPosition(0));
    
    float dist;
    if (plane.IntersectsRay(ray, &dist)) {
        Vector3 position = ray.GetPosition(dist);
    
        createdParticle->GetComponent<Particle>()->SetPosition(position);
        createdParticle->GetComponent<Transform>()->Position = position;
        createdParticle->AddComponent<Touchable>();
        
        Spring* spring = CreateSpring();
        spring->SetParticles(modifingParticle->GetComponent<Particle>(), createdParticle->GetComponent<Particle>());
        
        modifingParticle = createdParticle;
        
        createdParticle = CreateParticle();
    }
}
*/

void PhysicsEditor::Update(float dt) {
    if (!modifingParticle) return;
    
    Plane plane({0,0,1},0);
    
    Ray ray = cameraObject->GetComponent<Camera>()->GetRay(cameraObject->GetComponent<Transform>(), input->GetTouchPosition(0));
    
    float dist;
    if (plane.IntersectsRay(ray, &dist)) {
        Vector3 position = ray.GetPosition(dist);
        
        std::cout<<position << std::endl;
    
        createdParticle->GetComponent<Particle>()->SetPosition(position);
        createdParticle->GetComponent<Transform>()->Position = position;
    
        
        if (touchUp && !particleClicked) {
            
            Spring* spring = CreateSpring();
            spring->SetParticles(modifingParticle->GetComponent<Particle>(), createdParticle->GetComponent<Particle>());
        
            createdParticle->AddComponent<Touchable>();
        
            modifingParticle = createdParticle;
        
            createdParticle = CreateParticle();
            
        }
    }
    touchUp = false;
    particleClicked = false;
}




