//
//  FirstPersonMoverSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/28/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "FirstPersonMoverSystem.hpp"
#include "MathHelper.hpp"

using namespace Pocket;

FirstPersonMoverSystem::FirstPersonMoverSystem() : draggableSystem(0) {
    isDraggableSystemChecked = false;
}

void FirstPersonMoverSystem::Initialize() {
    root->Input().TouchDown.Bind(this, &FirstPersonMoverSystem::TouchDown);
    root->Input().TouchUp.Bind(this, &FirstPersonMoverSystem::TouchUp);
}

void FirstPersonMoverSystem::Destroy() {
    root->Input().TouchDown.Unbind(this, &FirstPersonMoverSystem::TouchDown);
    root->Input().TouchUp.Unbind(this, &FirstPersonMoverSystem::TouchUp);
}

void FirstPersonMoverSystem::TouchDown(Pocket::TouchEvent e) {
    if (draggableSystem && draggableSystem->IsTouchIndexUsed(e.Index)) return;
    touches[e.Index] = e.Position;
    
    if (e.Index==1) {
        const ObjectCollection& list = Objects();
        for (ObjectCollection::const_iterator it = list.begin(); it!=list.end(); ++it) {
            Transform* transform = (*it)->GetComponent<Transform>();
            FirstPersonMover* mover = (*it)->GetComponent<FirstPersonMover>();
            mover->initialRotation = transform->Rotation();
        }
    }
}

void FirstPersonMoverSystem::TouchUp(Pocket::TouchEvent e) {
    Touches::iterator it = touches.find(e.Index);
    if (it!=touches.end()) touches.erase(it);
}

void FirstPersonMoverSystem::Update(float dt) {
    if (!isDraggableSystemChecked) {
        isDraggableSystemChecked = true;
        draggableSystem = root->CreateSystem<DraggableSystem>(); // this was CreateSystem<>
    }
    
    for(auto o : Objects()) {
        if (!UpdateRotation(dt, o)) {
            UpdateMovement(dt, o);
        }
    }
}

void FirstPersonMoverSystem::UpdateMovement(float dt, GameObject* object) {
    int touchIndex = object->GetComponent<FirstPersonMover>()->MovementTouchIndex;
    if (draggableSystem && draggableSystem->IsTouchIndexUsed(touchIndex)) return;
    if (root->Input().IsTouchSwallowed(touchIndex, 0)) return;
    
    Touches::iterator it = touches.find(touchIndex);
    if (it==touches.end()) return;
    
    Vector2 delta = root->Input().GetTouchPosition(touchIndex) - it->second;
    
    Transform* transform = object->GetComponent<Transform>();
    FirstPersonMover* mover = object->GetComponent<FirstPersonMover>();
    Vector3 vector = transform->World().TransformVector(Vector3(delta.x * dt * mover->MovementSpeed,0,-delta.y * dt * mover->MovementSpeed));
    transform->Position += vector;
}

bool FirstPersonMoverSystem::UpdateRotation(float dt, GameObject* object) {
    int touchIndex = object->GetComponent<FirstPersonMover>()->RotationTouchIndex;
    if (root->Input().IsTouchSwallowed(touchIndex, 0)) return false;
    
    Touches::iterator it = touches.find(touchIndex);
    if (it==touches.end()) return false;
    
    Vector2 delta = root->Input().GetTouchPosition(touchIndex) - it->second;
    
    Transform* transform = object->GetComponent<Transform>();
    FirstPersonMover* mover = object->GetComponent<FirstPersonMover>();
    float maxPitch = ToRadians(mover->MaxPitch);
    float rotationSpeed = ToRadians(mover->RotationSpeed);
            
    float xRot = delta.y * rotationSpeed;
    float yRot = -delta.x * rotationSpeed;
    
    if (xRot<-maxPitch) {
        xRot = -maxPitch;
    } else if (xRot>maxPitch) {
        xRot = maxPitch;
    }
    
    Quaternion xAxis = Quaternion::Create(xRot, Vector3(1,0,0));
    Quaternion yAxis = Quaternion::Create(yRot, Vector3(0,1,0));
    
    Quaternion rot = yAxis * mover->initialRotation * xAxis;
    
    rot.Normalize();
    
    Vector3 euler = rot.ToEuler();
    float pitch = euler.x;
    if (pitch<-maxPitch) {
        rot = yAxis * mover->initialRotation * Quaternion::Create(-maxPitch-mover->initialRotation.ToEuler().x, Vector3(1,0,0));
    } else if (pitch>maxPitch) {
        Quaternion min;
        rot = yAxis * mover->initialRotation * Quaternion::Create(maxPitch-mover->initialRotation.ToEuler().x, Vector3(1,0,0));
    }
    
    transform->Rotation = rot;
	return true;
}






