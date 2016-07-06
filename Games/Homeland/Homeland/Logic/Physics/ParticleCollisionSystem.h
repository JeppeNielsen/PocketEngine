//
//  ParticleCollisionSystem.h
//  Homeland
//
//  Created by Jeppe Nielsen on 23/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Particle.h"
#include <cmath>
#include <iostream>
#include "Movable.h"

using namespace Pocket;

template<typename ...T>
class ParticleCollisionSystem : public GameSystem<Particle, T...> {
protected:

    void Update(float dt) override {
        
        //int hej = Objects().size();
        //hej++;
        /*
        for (int i=0; i<Objects().size(); i++) {
            Particle* a = Objects()[i]->GetComponent<Particle>();
            for (int j=i+1; j<Objects().size(); j++) {
                Particle* b = Objects()[j]->GetComponent<Particle>();
                
                const float radius = 2.05f;
                const float force = 0.1f;
                
                Vector2 vector = b->position - a->position;
                float length = vector.Length();
                if (length<radius) {
                    vector *= (1.0f / length);
                    float penetration = radius - length;
                    if (!a->immovable) {
                        a->position -= vector * penetration * 0.5f*force;
                    }
                    if (!b->immovable) {
                        b->position += vector * penetration * 0.5f*force;
                    }
                    
                    bool isAMoving = Objects()[i]->GetComponent<Movable>() ? Objects()[i]->GetComponent<Movable>()->path.size()>0 : false;
                    bool isBMoving = Objects()[j]->GetComponent<Movable>() ? Objects()[j]->GetComponent<Movable>()->path.size()>0 : false;
                    
                    if (isAMoving && !isBMoving) {
                        Vector2 normal = Vector2(-vector.y, vector.x);
                        if (normal.Dot(a->position - a->positionOld)>0) {
                            normal = -normal;
                        }
                        b->position += normal * penetration * 0.5f * force;
                    } else if (!isAMoving && isBMoving) {
                        Vector2 normal = Vector2(-vector.y, vector.x);
                        if (normal.Dot(b->position -b->positionOld)>0) {
                            normal = -normal;
                        }
                        a->position += normal * penetration * 0.5f*force;
                    }
                    
                }
            }
        }
         */
    }




};