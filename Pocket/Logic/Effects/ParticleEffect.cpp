//
//  Orderable.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/29/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "ParticleEffect.hpp"

using namespace Pocket;



void ParticleEffect::Reset() {
    MaxParticles = 100;
    Duration = 1.0f;
    EmissionFrequency = 1.0f;
    EmissionAmount = 1;
}

void ParticleEffect::Clone(const ParticleEffect& source) {
    Duration = source.Duration;
    EmissionFrequency = source.EmissionFrequency;
    EmissionAmount = source.EmissionAmount;
}

ParticleEffect::ParticleEffect()
:
MaxParticles(this), Duration(this), EmissionFrequency(this), EmissionAmount(this) { }

