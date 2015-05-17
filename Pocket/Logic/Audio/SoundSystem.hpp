//
//  SoundSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/30/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "Sound.hpp"
#include "SoundEmitter.hpp"
#include "SoundListener.hpp"
#include "Transform.hpp"

namespace Pocket {
    class SoundSystem : public GameSystem {
    public:
        SoundSystem();
        ~SoundSystem();
        
        void Initialize();
        void AddedToWorld(GameWorld& world);
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        void Update(float dt);
        
    private:
        
        static int counter;
        static ALCdevice* device;
        static ALCcontext* context;
        
        class ListenerSystem : public GameSystem {
        public:
            void Initialize();
            void ObjectAdded(GameObject* object);
            void ObjectRemoved(GameObject* object);
            void Update(float dt);
        private:
            Vector3 lastPosition;
            bool needsUpdate;
            void TransformChanged(DirtyProperty<Transform*, Matrix4x4>* worldProperty);
        };
        
        class SoundTransformSystem : public GameSystem {
        public:
            SoundSystem* soundSystem;
            void Initialize();
            void ObjectAdded(GameObject* object);
            void ObjectRemoved(GameObject* object);
        };
       
        class SoundableObject {
        public:
            
            void Initialize(GameObject* gameObject);
            void Destroy();
            
        private:
            Sound* sound;
            SoundEmitter* emitter;
            Transform* transform;
            ALuint alSource;
            
            void EmitterChanged(SoundEmitter* emitter);
            void PlayingChanged(SoundEmitter* emitter);
        public:
            void TransformAdded(Transform* transform);
            void TransformRemoved(Transform* transform);
        private:
            void TransformChanged(DirtyProperty<Transform*, Matrix4x4>* worldProperty);
            
        };
        
        
        
    
    };
}