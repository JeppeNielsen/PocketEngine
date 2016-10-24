//
//  ParticleMeshUpdater.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/14/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "GameSystem.hpp"
#include "ParticleEmitter.hpp"
#include "Mesh.hpp"

namespace Pocket {
    class ParticleMeshUpdater : public GameSystem<Mesh, ParticleEmitter> {
    public:
        void Update(float dt);
    private:
        void UpdateMesh(Mesh* mesh, ParticleEmitter* emitter);
    };
}