//
//  CreatorSystem.cpp
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 02/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "CreatorSystem.h"
#include "Plane.hpp"

void CreatorSystem::AddedToWorld(Pocket::GameWorld &world) {
    currentState = State::Idle;
    sourceParticle = 0;
    createdParticle = 0;
    createdSpring = 0;
    
    particles = world.CreateSystem<Particles>();
    springs = world.CreateSystem<Springs>();
}

void CreatorSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Click += event_handler(this, &CreatorSystem::ParticleClicked, object);
}

void CreatorSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Click -= event_handler(this, &CreatorSystem::ParticleClicked, object);
}

void CreatorSystem::ParticleClicked(Pocket::TouchData d, Pocket::GameObject *particleObject) {
    //Creator* creator = particleObject->GetComponent<Creator>();
    //if (!creator->particle || !creator->spring) return;
    if (currentState != State::Idle) return;
    currentTouch = d;
    currentTouch.Input->TouchDown += event_handler(this, &CreatorSystem::TouchDown);
    StartFromParticle(particleObject);
}

void CreatorSystem::StartFromParticle(Pocket::GameObject *particleObject) {
    sourceParticle = particleObject;
    createdParticle = CreateParticle(); //creator->particle->Clone();
    createdSpring = CreateSpring();// creator->spring->Clone();
    createdSpring->GetComponent<Spring>()->SetParticles(sourceParticle->GetComponent<Particle>(), createdParticle->GetComponent<Particle>());
    currentState = State::Creating;
}

void CreatorSystem::Update(float dt) {
    if (currentState == State::Idle) return;
    
    switch (currentState) {
        case State::Creating: {
            Plane plane({0,0,1},0);
            Ray ray = currentTouch.Camera->GetRay(
                    currentTouch.CameraObject->GetComponent<Transform>(),
                    currentTouch.Input->GetTouchPosition(currentTouch.Index));

            float dist;
            if (plane.IntersectsRay(ray, &dist)) {
                Vector3 position = Grid(ray.GetPosition(dist));
                createdParticle->GetComponent<Transform>()->Position = position;
                createdParticle->GetComponent<Particle>()->SetPosition(position);
            }
            
            createdSpring->GetComponent<Spring>()->SetLengthFromParticles();
            
            break;
        }
        case State::Succes: {
            bool continuous = Continuous();
            float minDistance;
            GameObject* nearestParticle = particles->FindNearestParticle(createdParticle->GetComponent<Particle>()->position, createdParticle, minDistance);
            if (minDistance<1.0f && nearestParticle) {
                createdParticle->Remove();
                if (sourceParticle!=nearestParticle &&
                    !springs->DoesSpringExists(sourceParticle->GetComponent<Particle>(), nearestParticle->GetComponent<Particle>())) {
                    createdSpring->GetComponent<Spring>()->SetParticles(
                        sourceParticle->GetComponent<Particle>(),
                        nearestParticle->GetComponent<Particle>()
                    );
                } else {
                    createdSpring->Remove();
                }
                if (continuous) {
                    StartFromParticle(nearestParticle);
                }
            } else {
                if (continuous) {
                    StartFromParticle(createdParticle);
                }
            }
            if (!continuous) {
                currentTouch.Input->TouchDown -= event_handler(this, &CreatorSystem::TouchDown);
                currentState = State::Idle;
            }
            
            break;
        }
        case State::Cancel: {
            createdParticle->Remove();
            createdSpring->Remove();
            currentTouch.Input->TouchDown -= event_handler(this, &CreatorSystem::TouchDown);
            currentState = State::Idle;
            break;
        }
    default:
        break;
    }
}

Vector3 CreatorSystem::Grid(Vector3 position) {
    position.x = roundf(position.x / gridSize.x) * gridSize.x;
    position.y = roundf(position.y / gridSize.y) * gridSize.y;
    return position;
}

void CreatorSystem::Cancel() {
    if (currentState!=State::Creating) return;
    currentState = State::Cancel;
}

void CreatorSystem::TouchDown(TouchEvent e) {
    if (e.Index == 0) {
        currentState = State::Succes;
    } else {
        currentState = State::Cancel;
    }
}

GameObject* CreatorSystem::Particles::FindNearestParticle(const Vector2& position, Pocket::GameObject *excludeThis, float &minDistance) {
    minDistance = 100000.0f;
    GameObject* found = 0;
    for(GameObject* o : Objects()) {
        if (o==excludeThis) continue;
        Particle* particle = o->GetComponent<Particle>();
        Vector2 toObject = particle->position - position;
        float length = toObject.Length();
        if (length<minDistance) {
            minDistance = length;
            found = o;
        }
    }
    return found;
}

bool CreatorSystem::Springs::DoesSpringExists(Particle *p1, Particle *p2) {
    for(GameObject* go : Objects()) {
        Spring* s = go->GetComponent<Spring>();
        if ((s->particleA == p1 && s->particleB == p2) || (s->particleA == p2 && s->particleB == p1)) {
            return true;
        }
    }
    return false;
}

