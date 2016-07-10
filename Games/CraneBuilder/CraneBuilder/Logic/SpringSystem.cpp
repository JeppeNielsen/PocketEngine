//
//  SpringSystem.cpp
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/4/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "SpringSystem.h"
#include "Particle.h"
#include <iostream>
using namespace Pocket;


void SpringSystem::ResetTension() {

    for (auto it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* object = *it;
        Spring* spring = object->GetComponent<Spring>();
        spring->tension = 0;
    }
}
    
void SpringSystem::Simulate(float dt) {

    //Objects()[0]->GetComponent<Spring>()->length += dt * 0.1f;
    for (auto it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* object = *it;

        Spring* spring = object->GetComponent<Spring>();
        if (!spring->particleA) continue;
        if (!spring->particleB) continue;
     
        Vector2 delta = spring->particleB->position - spring->particleA->position;
        
        spring->currentLength = delta.Length();
        float inverseLength = 1.0f/spring->currentLength;
        
        delta.x*=inverseLength;
        delta.y*=inverseLength;
        
        float totalMass = spring->particleA->mass + spring->particleB->mass;
        
        float m1 = spring->particleA->mass / totalMass;
        float m2 = 1.0f - m1;
        
        float displacement = (spring->currentLength - spring->length) * spring->elasticity;
        float dLength = displacement * dt;
        
        spring->tension += dLength;
        
        if (!spring->particleA->immovable) {
            spring->particleA->position += delta * dLength * m2;
        }
        if (!spring->particleB->immovable) {
            spring->particleB->position -= delta * dLength * m1;
        }
    }
}

void SpringSystem::CheckForBrokenSprings() {
    for (auto it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* object = *it;
        Spring* spring = object->GetComponent<Spring>();
        
        if (spring->tension>3.0f) {
            object->Remove();
        }
        
    }
}
