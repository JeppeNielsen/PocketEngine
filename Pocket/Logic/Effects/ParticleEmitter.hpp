//
//  ParticleEmitter.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/14/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include "Vector3.hpp"
#include "Colour.hpp"
#include <vector>

namespace Pocket {
    class ParticleEmitter {
    public:
        ParticleEmitter();
    
        Property<bool> UseWorldSpace;
        Property<bool> Playing;
        Property<float> Age;
    
    void CreateParticles(int amount);
    
    struct Particle {
        int id;
        Vector3 position;
        Vector3 velocity;
        Vector3 acceleration;
        float rotation;
        float angularVelocity;
        float size;
        float age;
        float lifetime;
        Colour color;
        int texture;
        float rnd[3];
        float velocityRnd[3];
        float accelerationRnd[3];
    };
    
    typedef std::vector<Particle> Particles;
    Particles particles;
    float frequencyTime;
    
    typedef std::vector<Particle*> DynamicParticles;
    DynamicParticles activeParticles;
    DynamicParticles inactiveParticles;
        
    };
}