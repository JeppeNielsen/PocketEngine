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
public:

    void Update(float dt) override {
        //this->Objects()[0]->template GetComponent<Movable>();
        for (int i=0; i<this->Objects().size(); i++) {
            Particle* a = this->Objects()[i]->template GetComponent<Particle>();
            for (int j=i+1; j<this->Objects().size(); j++) {
                Particle* b = this->Objects()[j]->template GetComponent<Particle>();
                
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
                    
                    bool isAMoving = this->Objects()[i]->template GetComponent<Movable>() ? this->Objects()[i]->template GetComponent<Movable>()->path.size()>0 : false;
                    bool isBMoving = this->Objects()[j]->template GetComponent<Movable>() ? this->Objects()[j]->template GetComponent<Movable>()->path.size()>0 : false;
                    
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
    }

};