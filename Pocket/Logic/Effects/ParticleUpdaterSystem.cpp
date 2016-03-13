//
//  ParticleUpdaterSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/14/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "ParticleUpdaterSystem.hpp"
#include "MathHelper.hpp"
#include "Transform.hpp"

#define RND_COLOR 0
#define RND_SIZE 1
#define RND_ROTATION 2

using namespace Pocket;

void ParticleUpdaterSystem::ObjectAdded(Pocket::GameObject *object) {
    ParticleEffect* effect = object->GetComponent<ParticleEffect>();
    ParticleEmitter* emitter = object->GetComponent<ParticleEmitter>();
    emitter->CreateParticles(effect->MaxParticles);
}

void ParticleUpdaterSystem::ObjectRemoved(Pocket::GameObject *object) {
    //ParticleEmitter* emitter = object->GetComponent<ParticleEmitter>();
    //emitter->CreateParticles(0);
}

void ParticleUpdaterSystem::Update(float dt) {
    for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* object = *it;
        UpdateEmitter(dt, object->GetComponent<ParticleEffect>(), object->GetComponent<ParticleEmitter>(), object);
    }
}

void ParticleUpdaterSystem::UpdateEmitter(float dt, ParticleEffect *effect, ParticleEmitter *emitter, GameObject* object) {
    
    Transform* transform = object->GetComponent<Transform>();
    bool useWorldSpace = transform && emitter->UseWorldSpace();
    
    if (emitter->Playing()) {
        float age = emitter->Age();
        if (age>effect->Duration()) {
            emitter->Playing = false;
        } else {
            if (emitter->frequencyTime>effect->EmissionFrequency()) {
                emitter->frequencyTime = 0;
                int amount = effect->EmissionAmount();
                
                for (int i=0; i<amount; ++i) {
                    if (emitter->inactiveParticles.empty()) break;
                    ParticleEmitter::Particle* particle = emitter->inactiveParticles.back();
                    emitter->inactiveParticles.pop_back();
                    particle->id = (int)emitter->activeParticles.size();
                    particle->position = useWorldSpace ? transform->World().TransformPosition(Vector3(0,0,0)) : Vector3(0,0,0);
                    particle->rotation = effect->Rotation.Get(MathHelper::Random());
                    particle->age = 0;
                    particle->lifetime = effect->Lifetime.Get(MathHelper::Random());
                    particle->texture = 0;
                    particle->rnd[RND_COLOR]=MathHelper::Random();
                    particle->rnd[RND_SIZE]=MathHelper::Random();
                    particle->rnd[RND_ROTATION]=MathHelper::Random();
                    
                    particle->velocityRnd[0]=MathHelper::Random();
                    particle->velocityRnd[1]=MathHelper::Random();
                    particle->velocityRnd[2]=MathHelper::Random();
                    particle->accelerationRnd[0]=MathHelper::Random();
                    particle->accelerationRnd[1]=MathHelper::Random();
                    particle->accelerationRnd[2]=MathHelper::Random();
                    
                    
                    if (effect->Size.Count()==0) {
                        particle->size = 1.0f;
                    } else if (effect->Size.Count()==1) {
                        const Span<float>& size = effect->Size.Get(0).Source;
                        particle->size = size.Get(particle->rnd[RND_SIZE]);
                    }
                    
                    if (effect->RotationSpeed.Count()==0) {
                        particle->angularVelocity = 0;
                    } else if (effect->RotationSpeed.Count()==1) {
                        const Span<float>& angularVelocity = effect->RotationSpeed.Get(0).Source;
                        particle->angularVelocity = angularVelocity.Get(particle->rnd[RND_ROTATION]);
                    }
                    
                    
                    
                    if (effect->Velocity.Count()==0) {
                        particle->velocity = Vector3(0,0,0);
                    } else if (effect->Velocity.Count() == 1) {
                        const Span<Vector3>& velocity = effect->Velocity.Get(0).Source;
                        particle->velocity = Vector3(
                                                     velocity.Min.x + (velocity.Max.x - velocity.Min.x) * particle->velocityRnd[0],
                                                     velocity.Min.y + (velocity.Max.y - velocity.Min.y) * particle->velocityRnd[1],
                                                     velocity.Min.z + (velocity.Max.z - velocity.Min.z) * particle->velocityRnd[2]);
                        if (useWorldSpace) transform->World().TransformVector(particle->velocity, particle->velocity);
                    }
                    
                    if (effect->Acceleration.Count()==0) {
                        particle->acceleration = Vector3(0,0,0);
                    } else if (effect->Acceleration.Count() == 1) {
                        const Span<Vector3>& acceleration = effect->Acceleration.Get(0).Source;
                        particle->acceleration = Vector3(
                                                     acceleration.Min.x + (acceleration.Max.x - acceleration.Min.x) * particle->accelerationRnd[0],
                                                     acceleration.Min.y + (acceleration.Max.y - acceleration.Min.y) * particle->accelerationRnd[1],
                                                     acceleration.Min.z + (acceleration.Max.z - acceleration.Min.z) * particle->accelerationRnd[2]);
                    }
                    
                    if (effect->Color.Count() == 0) {
                        particle->color =  Colour::White();
                    } else if(effect->Color.Count() == 1) {
                        const Span<Colour>& color = effect->Color.Get(0).Source;
                        particle->color = Colour::Lerp(color.Min, color.Max, particle->rnd[RND_COLOR]);
                    }
                                        
                    emitter->activeParticles.push_back(particle);
                }
            } else {
                emitter->frequencyTime += dt;
            }
            
            emitter->Age = age + dt;
        }
    }
    
    
    for (size_t i=0; i<emitter->activeParticles.size(); ++i) {
        ParticleEmitter::Particle* particle = emitter->activeParticles[i];
        
        while (true) {
            if (particle->age>particle->lifetime) {
                ParticleEmitter::Particle* last = emitter->activeParticles.back();
                emitter->activeParticles[particle->id] = last;
                last->id = particle->id;
                emitter->activeParticles.pop_back();
                emitter->inactiveParticles.push_back(particle);
                if (particle == last) return;
                particle = last;
            } else {
                break;
            }
        }
        
        float age = particle->age / particle->lifetime;
        
        if (effect->Size.Count()>1) {
            const NodeLine<Span<float> >::Result& result = effect->Size.Get(age);
            float sizeA = result.Source.Get(particle->rnd[RND_SIZE]);
            float sizeB = result.Dest.Get(particle->rnd[RND_SIZE]);
            particle->size = sizeA + (sizeB - sizeA) * result.t;
        }
        
        if (effect->RotationSpeed.Count()>1) {
            const NodeLine<Span<float> >::Result& result = effect->RotationSpeed.Get(age);
            float angularVelocityA = result.Source.Get(particle->rnd[RND_ROTATION]);
            float angularVelocityB = result.Dest.Get(particle->rnd[RND_ROTATION]);
            particle->angularVelocity = angularVelocityA + (angularVelocityB - angularVelocityA) * result.t;
        }
        
        if (effect->Velocity.Count()>1) {
            const NodeLine<Span<Vector3> >::Result& result = effect->Velocity.Get(age);
            Vector3 velocityA = Vector3(result.Source.Min.x + (result.Source.Max.x - result.Source.Min.x) * particle->velocityRnd[0],
                                        result.Source.Min.y + (result.Source.Max.y - result.Source.Min.y) * particle->velocityRnd[1],
                                        result.Source.Min.z + (result.Source.Max.z - result.Source.Min.z) * particle->velocityRnd[2]);
            
            Vector3 velocityB = Vector3(result.Dest.Min.x + (result.Dest.Max.x - result.Dest.Min.x) * particle->velocityRnd[0],
                                        result.Dest.Min.y + (result.Dest.Max.y - result.Dest.Min.y) * particle->velocityRnd[1],
                                        result.Dest.Min.z + (result.Dest.Max.z - result.Dest.Min.z) * particle->velocityRnd[2]);
            
            particle->velocity = Vector3(
                                         velocityA.x + (velocityB.x - velocityA.x) * result.t,
                                         velocityA.y + (velocityB.y - velocityA.y) * result.t,
                                         velocityA.z + (velocityB.z - velocityA.z) * result.t);
        }
        
        if (effect->Acceleration.Count()>1) {
            const NodeLine<Span<Vector3> >::Result& result = effect->Acceleration.Get(age);
            Vector3 accelerationA = Vector3(result.Source.Min.x + (result.Source.Max.x - result.Source.Min.x) * particle->accelerationRnd[0],
                                        result.Source.Min.y + (result.Source.Max.y - result.Source.Min.y) * particle->accelerationRnd[1],
                                        result.Source.Min.z + (result.Source.Max.z - result.Source.Min.z) * particle->accelerationRnd[2]);
            
            Vector3 accelerationB = Vector3(result.Dest.Min.x + (result.Dest.Max.x - result.Dest.Min.x) * particle->accelerationRnd[0],
                                        result.Dest.Min.y + (result.Dest.Max.y - result.Dest.Min.y) * particle->accelerationRnd[1],
                                        result.Dest.Min.z + (result.Dest.Max.z - result.Dest.Min.z) * particle->accelerationRnd[2]);
            
            particle->acceleration = Vector3(
                                         accelerationA.x + (accelerationB.x - accelerationA.x) * result.t,
                                         accelerationA.y + (accelerationB.y - accelerationA.y) * result.t,
                                         accelerationA.z + (accelerationB.z - accelerationA.z) * result.t);
        }
        
        if (effect->Color.Count()>1) {
            const NodeLine<Span<Colour> >::Result& result = effect->Color.Get(age);
            Colour colorA = Colour::Lerp(result.Source.Min, result.Source.Max, particle->rnd[RND_COLOR]);
            Colour colorB = Colour::Lerp(result.Dest.Min, result.Dest.Max, particle->rnd[RND_COLOR]);
            particle->color = Colour::Lerp(colorA, colorB, result.t);
        }
        
        particle->velocity += particle->acceleration * dt;
        particle->position += particle->velocity * dt;
        particle->rotation += particle->angularVelocity * dt;
        
        particle->age += dt;
    }
}
