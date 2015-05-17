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

FirstPersonMoverSystem::FirstPersonMoverSystem() : Input(this), draggableSystem(0) {
    Input = 0;
    Input.ChangedWithOld += event_handler(this, &FirstPersonMoverSystem::InputChanged);
    isDraggableSystemChecked = false;
}

void FirstPersonMoverSystem::Initialize() {
    AddComponent<Transform>();
    AddComponent<FirstPersonMover>();
}

void FirstPersonMoverSystem::InputChanged(Property<FirstPersonMoverSystem *, InputManager *>::EventData e) {
    if (e.Old) {
        e.Old->TouchDown -= event_handler(this, &FirstPersonMoverSystem::TouchDown);
        e.Old->TouchUp -= event_handler(this, &FirstPersonMoverSystem::TouchUp);
        touches.clear();
    }
    
    if (e.Current) {
        e.Current->TouchDown += event_handler(this, &FirstPersonMoverSystem::TouchDown);
        e.Current->TouchUp += event_handler(this, &FirstPersonMoverSystem::TouchUp);
    }
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
        draggableSystem = World()->GetSystem<DraggableSystem>();
    }
    
    if (!UpdateRotation(dt,1)) {
        UpdateMovement(dt,0);
    }
}

void FirstPersonMoverSystem::UpdateMovement(float dt, int touchIndex) {
    if (draggableSystem && draggableSystem->IsTouchIndexUsed(touchIndex)) return;
    
    Touches::iterator it = touches.find(0);
    if (it==touches.end()) return;
    
    Vector2 delta = Input()->GetTouchPosition(0) - it->second;
    
    const ObjectCollection& list = Objects();
    
	for (ObjectCollection::const_iterator it = list.begin(); it!=list.end(); ++it) {
		Transform* transform = (*it)->GetComponent<Transform>();
        FirstPersonMover* mover = (*it)->GetComponent<FirstPersonMover>();
		Vector3 vector = transform->World.GetValue()->TransformVector(Vector3(delta.x * dt * mover->MovementSpeed,0,-delta.y * dt * mover->MovementSpeed));
		transform->Position += vector;
	}
}

bool FirstPersonMoverSystem::UpdateRotation(float dt, int touchIndex) {
    
    Touches::iterator it = touches.find(touchIndex);
    if (it==touches.end()) return false;
    
    Vector2 delta = Input()->GetTouchPosition(touchIndex) - it->second;
    
    const ObjectCollection& list = Objects();
    
	for (ObjectCollection::const_iterator it = list.begin(); it!=list.end(); ++it) {
		Transform* transform = (*it)->GetComponent<Transform>();
        FirstPersonMover* mover = (*it)->GetComponent<FirstPersonMover>();
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
	}
    
    return true;
}






