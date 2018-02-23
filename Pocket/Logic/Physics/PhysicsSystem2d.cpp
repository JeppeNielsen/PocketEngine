//
//  PhysicsSystem2d.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 12/28/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "PhysicsSystem2d.hpp"
#include "b2World.h"
#include "Collidable.hpp"

using namespace Pocket;

PhysicsSystem2d::PhysicsSystem2d() {
    b2AABB bounds;
    bounds.lowerBound = b2Vec2(-500,-100);
    bounds.upperBound = b2Vec2(500,100);
    
    physicsWorld = new b2World(bounds, b2Vec2(0,-20), true);
    physicsWorld->SetContactListener(&contactListener);
    
    isSimulating = false;
}

PhysicsSystem2d::~PhysicsSystem2d() {
    delete physicsWorld;
}

void PhysicsSystem2d::CreateSubSystems(Pocket::GameStorage &storage) {
    storage.AddSystemType<JointSystem>();
}

void PhysicsSystem2d::Initialize() {
    jointSystem = root->CreateSystem<JointSystem>();
    jointSystem->physicsSystem = this;
}

void PhysicsSystem2d::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Transform>()->Position.Changed.Bind(this, &PhysicsSystem2d::TransformChanged, object);
    RigidBody2d* rigidBody = object->GetComponent<RigidBody2d>();
    rigidBody->HasBecomeDirty.Bind(this, &PhysicsSystem2d::BodyWasDirty, object);
    BodyWasDirty(rigidBody, object);
}

void PhysicsSystem2d::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Transform>()->Position.Changed.Unbind(this, &PhysicsSystem2d::TransformChanged, object);
    RigidBody2d* rigidBody = object->GetComponent<RigidBody2d>();
    rigidBody->HasBecomeDirty.Unbind(this, &PhysicsSystem2d::BodyWasDirty, object);
    
    deletedBodies.insert(object);
    DirtyBodies::iterator it = dirtyBodies.find(object);
    if (it!=dirtyBodies.end()) dirtyBodies.erase(it);
}

void PhysicsSystem2d::TransformChanged(GameObject* object) {
    if (isSimulating) return;
    Transform* transform = object->GetComponent<Transform>();
    Vector3 position = transform->Position;
    RigidBody2d* rigidBody = object->GetComponent<RigidBody2d>();
    rigidBody->body->SetXForm(b2Vec2(position.x, position.y), transform->Rotation().ToEuler().z);
}

void PhysicsSystem2d::BodyWasDirty(RigidBody2d* rigidBody, GameObject* object) {
    dirtyBodies.insert(object);
}

void PhysicsSystem2d::CreateBody(GameObject* object, Pocket::RigidBody2d *rigidBody, Transform* transform) {

    if (rigidBody->body) {
        physicsWorld->DestroyBody(rigidBody->body);
    }
    
    rigidBody->body = physicsWorld->CreateBody(&rigidBody->bodyDefinition);
    
    for (int i=0; i<rigidBody->ReadOnlyPolygons().size(); i++) {
        b2ShapeDef* shape = (b2ShapeDef*)&rigidBody->ReadOnlyPolygons()[i];
        rigidBody->body->CreateShape(shape);
    }
    
    for (int i=0; i<rigidBody->ReadOnlyCircles().size(); i++) {
        b2ShapeDef* shape = (b2ShapeDef*)&rigidBody->ReadOnlyCircles()[i];
        rigidBody->body->CreateShape(shape);
    }
    
    rigidBody->body->SetMassFromShapes();
    Vector3 position = transform->Position;
    rigidBody->body->SetXForm(b2Vec2(position.x, position.y), transform->Rotation().ToEuler().z);
    rigidBody->body->SetUserData(object);
}

void PhysicsSystem2d::Update(float dt) {
    
    jointSystem->DeleteJoints();
    
    if (!deletedBodies.empty()) {
        for (DirtyBodies::iterator it = deletedBodies.begin(); it!=deletedBodies.end(); ++it) {
            GameObject* go = *it;
            RigidBody2d* rigidBody = go->GetComponent<RigidBody2d>();
            physicsWorld->DestroyBody(rigidBody->body);
            rigidBody->body = 0;
        }
        deletedBodies.clear();
    }
    
    if (!dirtyBodies.empty()) {
        for (DirtyBodies::iterator it = dirtyBodies.begin(); it!=dirtyBodies.end(); ++it) {
            GameObject* go = *it;
            CreateBody(go, go->GetComponent<RigidBody2d>(), go->GetComponent<Transform>());
        }
        dirtyBodies.clear();
    }
    
    jointSystem->CreateJoints();
    
    isSimulating = true;
    physicsWorld->Step(dt * 2, 10);
    for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* go = *it;
        RigidBody2d* rigidBody = go->GetComponent<RigidBody2d>();
        if (rigidBody->body->IsSleeping()) continue;
        Transform* transform = go->GetComponent<Transform>();
       
        const b2Vec2 position = rigidBody->body->GetPosition();
        float rotation = rigidBody->body->GetAngle();
        Vector3 transformPosition = transform->Position;
        transformPosition.x = position.x;
        transformPosition.y = position.y;
        transform->Position = transformPosition;
        transform->Rotation = rotation;
    }
    isSimulating = false;
    
    //cout<<"Body count " << physicsWorld->GetBodyCount() << " Joint count " << physicsWorld->GetJointCount() << endl;
}


void PhysicsSystem2d::JointSystem::ObjectAdded(Pocket::GameObject *object) {
    jointsToCreate.push_back(object->GetComponent<Joint2d>());
}

void PhysicsSystem2d::JointSystem::ObjectRemoved(Pocket::GameObject *object) {
    Joint2d* joint2d = object->GetComponent<Joint2d>();
    jointsToDelete.push_back(joint2d);
}

void PhysicsSystem2d::JointSystem::CreateJoints() {
    if (jointsToCreate.empty()) return;
    for (size_t i = 0; i<jointsToCreate.size(); ++i) {
        Joint2d* joint2d = jointsToCreate[i];
        joint2d->jointDef->body1 = joint2d->BodyA()->body;
        joint2d->jointDef->body2 = joint2d->BodyB()->body;
        joint2d->joint = physicsSystem->physicsWorld->CreateJoint(joint2d->jointDef);
    }
    jointsToCreate.clear();
}

void PhysicsSystem2d::JointSystem::DeleteJoints() {
    if (jointsToDelete.empty()) return;
    for (size_t i = 0; i<jointsToDelete.size(); ++i) {
        Joint2d* joint2d = jointsToDelete[i];
        if (joint2d->joint) {
            physicsSystem->physicsWorld->DestroyJoint(joint2d->joint);
            joint2d->joint = 0;
        }
    }
    jointsToDelete.clear();
}

void PhysicsSystem2d::ContactListener::Add(const b2ContactPoint* point) {
    GameObject* objectA = static_cast<GameObject*>( point->shape1->GetBody()->GetUserData() );
    GameObject* objectB = static_cast<GameObject*>( point->shape2->GetBody()->GetUserData() );
    
    Collidable* collidableA = objectA->GetComponent<Collidable>();
    Collidable* collidableB = objectB->GetComponent<Collidable>();
    
    if (collidableA) {
        if (collidableA->collisionCounts==0) {
            collidableA->IsColliding = true;
        }
        collidableA->collisionCounts++;
        collidableA->Enter({ objectA, collidableA, objectB, collidableB });
    }
    
    if (collidableB) {
        if (collidableB->collisionCounts==0) {
            collidableB->IsColliding = true;
        }
        collidableB->collisionCounts++;
        collidableB->Enter({ objectB, collidableB, objectA, collidableA });
    }
}

void PhysicsSystem2d::ContactListener::Remove(const b2ContactPoint* point) {

   GameObject* objectA = static_cast<GameObject*>( point->shape1->GetBody()->GetUserData() );
    GameObject* objectB = static_cast<GameObject*>( point->shape2->GetBody()->GetUserData() );
    
    Collidable* collidableA = objectA->GetComponent<Collidable>();
    Collidable* collidableB = objectB->GetComponent<Collidable>();

    if (collidableA) {
        collidableA->collisionCounts--;
        if (collidableA->collisionCounts==0) {
            collidableA->IsColliding = false;
        }
        collidableA->Exit({ objectA, collidableA, objectB, collidableB });
    }
    
    if (collidableB) {
        collidableB->collisionCounts--;
        if (collidableB->collisionCounts==0) {
            collidableB->IsColliding = false;
        }
        collidableB->Exit({ objectB, collidableB, objectA, collidableA });
    }

}





