//
//  DraggableVelocitySystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/10/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "DraggableMotionSystem.hpp"

using namespace Pocket;


void DraggableMotionSystem::Initialize() {
    AddComponent<Transform>();
    AddComponent<Velocity>();
    AddComponent<Draggable>();
}

void DraggableMotionSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Draggable>()->IsDragging.Changed += event_handler(this, &DraggableMotionSystem::DraggingChanged, object);
}

void DraggableMotionSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Draggable>()->IsDragging.Changed -= event_handler(this, &DraggableMotionSystem::DraggingChanged, object);
}

void DraggableMotionSystem::DraggingChanged(Pocket::Draggable *draggable, Pocket::GameObject *object) {
    if (draggable->IsDragging) {
        draggingObjects[draggable] = { object, object->GetComponent<Transform>()->Position };
    } else {
        DraggingObjects::iterator it = draggingObjects.find(draggable);
        
        if (it!=draggingObjects.end()) {
            if (!it->second.velocities.empty()) {
                Vector3 velocity(0,0,0);
                for (int i=0; i<it->second.velocities.size(); ++i) {
                    velocity += it->second.velocities[i];
                }
                velocity /= it->second.velocities.size();
                object->GetComponent<Velocity>()->velocity = velocity;
            }
            draggingObjects.erase(it);
        }
    }
}

void DraggableMotionSystem::Update(float dt) {
    
    for (DraggingObjects::iterator it = draggingObjects.begin(); it!=draggingObjects.end(); ++it) {
        Transform* transform = it->second.object->GetComponent<Transform>();
        Vector3 position = transform->Position;
        
        Vector3 velocity = (position-it->second.lastPosition) / dt;
        it->second.velocities.push_back(velocity);
        while (it->second.velocities.size()>6) {
            it->second.velocities.erase(it->second.velocities.begin());
        }
        it->second.lastPosition = position;
    }
    
    
}