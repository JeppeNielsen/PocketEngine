//
//  PhysicsSystem2d.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 12/28/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once


#include "GameWorld.hpp"
#include "Transform.hpp"
#include "RigidBody2d.hpp"
#include "Joint2d.hpp"
#include <set>
#include "b2WorldCallbacks.h"

class b2World;

namespace Pocket {
    class PhysicsSystem2d : public GameSystem {
    public:
        PhysicsSystem2d();
        ~PhysicsSystem2d();
        void Initialize();
        void AddedToWorld(GameWorld& world);
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        void Update(float dt);
    private:
        b2World* physicsWorld;
        
        void TransformChanged(Transform* transform, GameObject* object);
        
        bool isSimulating;
        
        class JointSystem : public GameSystem {
        
        public:
            
            PhysicsSystem2d* physicsSystem;
            
            void Initialize();
            void ObjectAdded(GameObject* object);
            void ObjectRemoved(GameObject* object);
            
            void CreateJoints();
            void DeleteJoints();
            
            typedef std::vector<Joint2d*> JointList;
        private:
            JointList jointsToCreate;
            JointList jointsToDelete;
            
        };
        
        JointSystem* jointSystem;
        
        typedef std::set<GameObject*> DirtyBodies;
        DirtyBodies dirtyBodies;
        
        DirtyBodies deletedBodies;
        
        void BodyWasDirty(RigidBody2d* rigidBody, GameObject* object);
        
        void CreateBody(GameObject* object, RigidBody2d* rigidBody, Transform* transform);
        
        struct ContactListener : public b2ContactListener {
            void Add(const b2ContactPoint* point);
            void Remove(const b2ContactPoint* point);
        };
        
        ContactListener contactListener;
    };
}