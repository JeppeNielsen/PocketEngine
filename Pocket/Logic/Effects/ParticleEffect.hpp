//
//  Orderable.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/29/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameComponent.hpp"
#include "Property.hpp"
#include "BezierCurve1.hpp"
#include "Span.hpp"
#include "Colour.hpp"
#include "Vector3.hpp"

namespace Pocket {
    Component(ParticleEffect)
    public:
        ParticleEffect();
    
        void Reset();
        void Clone(const ParticleEffect& source);
    
        Property<ParticleEffect*, float> MaxParticles;
        Property<ParticleEffect*, float> Duration;
        Property<ParticleEffect*, float> EmissionFrequency;
        Property<ParticleEffect*, int> EmissionAmount;
    
        Span<float> Lifetime;
    
        Span<float> Rotation;
        NodeLine<Span<float> > RotationSpeed;
    
        NodeLine<Span<float> > Size;
    
        NodeLine<Span<Vector3> > Velocity;
        NodeLine<Span<Vector3> > Acceleration;
    
        NodeLine<Span<Colour> > Color;
       
    
    
    };
}