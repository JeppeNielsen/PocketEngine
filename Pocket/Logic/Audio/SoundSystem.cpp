//
//  SoundSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/30/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "SoundSystem.hpp"

using namespace Pocket;

int SoundSystem::counter = 0;
ALCdevice* SoundSystem::device = 0;
ALCcontext* SoundSystem::context = 0;

SoundSystem::SoundSystem() {
    if (counter==0) {
        device = alcOpenDevice(NULL);
        if(device==NULL)
        {
            std::cout << "cannot open sound card" << std::endl;
            return;
        }
        context=alcCreateContext(device,NULL);
        if(context==NULL) {
            std::cout << "cannot open context" << std::endl;
            return;
        }
        alcMakeContextCurrent(context);
        
        alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
    }
    counter++;
}

SoundSystem::~SoundSystem() {
    counter--;
    if (counter == 0) {
        alcDestroyContext(context);
        alcCloseDevice(device);
    }
}

void SoundSystem::CreateSubSystems(Pocket::GameStorage &storage) {
    storage.AddSystemType<ListenerSystem>();
    storage.AddSystemType<SoundTransformSystem>();
}

void SoundSystem::Initialize() {
    float f[]={0,0,-1,0,1,0};
    alListenerfv(AL_ORIENTATION, f);
    root->CreateSystem<ListenerSystem>();
    SoundTransformSystem* transformSystem = root->CreateSystem<SoundTransformSystem>();
    transformSystem->soundSystem = this;
}

void SoundSystem::ObjectAdded(GameObject* object) {
    SoundableObject* soundableObject = new SoundableObject();
    soundableObject->Initialize(object);
    
    SetMetaData(object, soundableObject);
}

void SoundSystem::ObjectRemoved(GameObject* object) {
    SoundableObject* soundableObject = (SoundableObject*)GetMetaData(object);
    soundableObject->Destroy();
    delete soundableObject;
}


void SoundSystem::Update(float dt) {
    
}

void SoundSystem::ListenerSystem::Initialize() {
    needsUpdate = false;
}

void SoundSystem::ListenerSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Transform>()->World.HasBecomeDirty.Bind(this, &SoundSystem::ListenerSystem::TransformChanged);
    needsUpdate = true;
    lastPosition = object->GetComponent<Transform>()->World().Translation();
   // AL_MAX_DISTANCE
    alListenerf(AL_MAX_DISTANCE, 5.0f);
}

void SoundSystem::ListenerSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Transform>()->World.HasBecomeDirty.Unbind(this, &SoundSystem::ListenerSystem::TransformChanged);
    if (Objects().size()==0) {
        float f[]={0,0,-1,0,1,0};
        alListenerfv(AL_ORIENTATION, f);
    }
}

void SoundSystem::ListenerSystem::TransformChanged() {
    needsUpdate = true;
}

void SoundSystem::ListenerSystem::Update(float dt) {
    if (!needsUpdate) return;
    if (Objects().size()<1) return;
    needsUpdate = false;
    GameObject* listener = Objects()[0];
    
    const Matrix4x4& world = listener->GetComponent<Transform>()->World;
    
    float invDt = 1.0f / dt;
    
    Vector3 position = world.Translation();
    Vector3 velocity = (position - lastPosition) * invDt * 0.1f;
    lastPosition = position;
    Vector3 forward = world.TransformVector(Vector3(0,0,-1));
    Vector3 up = world.TransformVector(Vector3(0,1,0));
    
    ALfloat listenerPos[] = { position.x, position.y, position.z };
    ALfloat listenerVel[] = { velocity.x, velocity.y, velocity.z };
    ALfloat listenerOri[] = { forward.x, forward.y, forward.z, up.x, up.y, up.z };
    
    alListenerfv(AL_POSITION, listenerPos);
    alListenerfv(AL_VELOCITY, listenerVel);
    alListenerfv(AL_ORIENTATION, listenerOri);
}

void SoundSystem::SoundTransformSystem::ObjectAdded(GameObject* object) {
    SoundableObject* soundableObject = (SoundableObject*)soundSystem->GetMetaData(object);
    soundableObject->TransformAdded(object->GetComponent<Transform>());
}

void SoundSystem::SoundTransformSystem::ObjectRemoved(GameObject* object) {
    SoundableObject* soundableObject = (SoundableObject*)soundSystem->GetMetaData(object);
    soundableObject->TransformRemoved(object->GetComponent<Transform>());
}

void SoundSystem::SoundableObject::Initialize(Pocket::GameObject* object) {
    sound = object->GetComponent<Sound>();
    emitter = object->GetComponent<SoundEmitter>();
    transform = object->GetComponent<Transform>();

    alGenSources(1, &alSource);
    
    alSource3f(alSource,AL_POSITION, 0, 0, 0);
    alSourcei(alSource, AL_SOURCE_RELATIVE, AL_TRUE);
    
    emitter->Playing.Changed.Bind(this, &SoundSystem::SoundableObject::PlayingChanged);
    emitter->Pitch.Changed.Bind(this, &SoundSystem::SoundableObject::EmitterChanged);
    emitter->Volume.Changed.Bind(this, &SoundSystem::SoundableObject::EmitterChanged);
    emitter->Looping.Changed.Bind(this, &SoundSystem::SoundableObject::EmitterChanged);
    emitter->MinDistance.Changed.Bind(this, &SoundSystem::SoundableObject::EmitterChanged);
    emitter->MaxDistance.Changed.Bind(this, &SoundSystem::SoundableObject::EmitterChanged);
    
    EmitterChanged();
    PlayingChanged();
}

void SoundSystem::SoundableObject::Destroy() {
    alDeleteSources(1, &alSource);
    alSource = 0;
    
    emitter->Playing.Changed.Unbind(this, &SoundSystem::SoundableObject::PlayingChanged);
    emitter->Pitch.Changed.Unbind(this, &SoundSystem::SoundableObject::EmitterChanged);
    emitter->Volume.Changed.Unbind(this, &SoundSystem::SoundableObject::EmitterChanged);
    emitter->Looping.Changed.Unbind(this, &SoundSystem::SoundableObject::EmitterChanged);
    emitter->MinDistance.Changed.Unbind(this, &SoundSystem::SoundableObject::EmitterChanged);
    emitter->MaxDistance.Changed.Unbind(this, &SoundSystem::SoundableObject::EmitterChanged);
}

void SoundSystem::SoundableObject::EmitterChanged() {
    alSourcei(alSource, AL_BUFFER, sound->GetOpenALBuffer());
    alSourcef(alSource, AL_PITCH, emitter->Pitch());
    alSourcef(alSource, AL_GAIN, emitter->Volume());
    alSourcei(alSource, AL_LOOPING, emitter->Looping() ? AL_TRUE : AL_FALSE);
    alSourcef(alSource, AL_REFERENCE_DISTANCE, emitter->MinDistance());
    alSourcef(alSource, AL_MAX_DISTANCE, emitter->MaxDistance());
    
}

void SoundSystem::SoundableObject::PlayingChanged() {
    if (emitter->Playing()) {
        alSourcePlay(alSource);
    } else {
        alSourcePause(alSource);
    }
}

void SoundSystem::SoundableObject::TransformAdded(Pocket::Transform *transform) {
    transform->World.HasBecomeDirty.Bind(this, &SoundSystem::SoundableObject::TransformChanged);
    alSourcei(alSource, AL_SOURCE_RELATIVE, AL_FALSE);
    TransformChanged();
}

void SoundSystem::SoundableObject::TransformRemoved(Pocket::Transform *transform) {
    transform->World.HasBecomeDirty.Unbind(this, &SoundSystem::SoundableObject::TransformChanged);
    alSource3f(alSource,AL_POSITION, 0, 0, 0);
    alSourcei(alSource, AL_SOURCE_RELATIVE, AL_TRUE);
}

void SoundSystem::SoundableObject::TransformChanged() {
    Vector3 position = transform->World().Translation();
    alSource3f(alSource,AL_POSITION,position.x,position.y,position.z);
}


