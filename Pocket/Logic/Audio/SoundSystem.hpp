//
//  SoundSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/30/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameSystem.hpp"
#include "Sound.hpp"
#include "SoundEmitter.hpp"
#include "SoundListener.hpp"
#include "Transform.hpp"

namespace Pocket {
    class SoundSystem : public GameSystem<Sound, SoundEmitter> {
    public:
        SoundSystem();
        ~SoundSystem();
        
        
        void Initialize();
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        void Update(float dt);
        static void CreateSubSystems(GameStorage& storage);
        
    private:
        
        static int counter;
        static ALCdevice* device;
        static ALCcontext* context;
        
        class ListenerSystem : public GameSystem<SoundListener, Transform> {
        public:
            void Initialize();
            void ObjectAdded(GameObject* object);
            void ObjectRemoved(GameObject* object);
            void Update(float dt);
        private:
            Vector3 lastPosition;
            bool needsUpdate;
            void TransformChanged();
        };
        
        class SoundTransformSystem : public GameSystem<Sound, SoundEmitter, Transform> {
        public:
            SoundSystem* soundSystem;
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
            
            void EmitterChanged();
            void PlayingChanged();
        public:
            void TransformAdded(Transform* transform);
            void TransformRemoved(Transform* transform);
        private:
            void TransformChanged();
            
        };
    };
}
