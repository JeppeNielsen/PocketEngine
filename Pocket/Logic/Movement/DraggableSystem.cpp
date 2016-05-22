//
//  DraggerSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/12/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "DraggableSystem.hpp"
#include "InputManager.hpp"
#include "Camera.hpp"

using namespace Pocket;

void DraggableSystem::ObjectAdded(GameObject *object) {
    Touchable* touchable = object->GetComponent<Touchable>();
    touchable->Down.Bind(this, &DraggableSystem::Down, object);
    touchable->Up.Bind(this, &DraggableSystem::Up, object);
}

void DraggableSystem::ObjectRemoved(GameObject *object) {
    Touchable* touchable = object->GetComponent<Touchable>();
    touchable->Down.Unbind(this, &DraggableSystem::Down, object);
    touchable->Up.Unbind(this, &DraggableSystem::Up, object);
    TouchData d;
    d.Touchable = touchable;
    Up(d, object);
}

void DraggableSystem::Update(float dt) {
    
    for (auto it = draggingObjects.begin(); it!=draggingObjects.end(); ++it) {
        
        DraggingObject& d = it->second;
        Vector2 touchPosition = d.touch.Input->GetTouchPosition(d.touch.Index);
        
        Ray ray = d.touch.Camera->GetRay(d.touch.CameraTransform, touchPosition);
        
        float distance;
        
        if (d.dragPlane.IntersectsRay(ray, &distance)) {
            Vector3 worldPosition = ray.position + ray.direction * distance;
            Vector3 localPosition = d.transform->WorldInverse().TransformPosition(worldPosition);
            
            Vector3 local = localPosition - d.offset + d.transform->Anchor;
            
            Vector3 currentPosition = local;
            
            switch (d.draggable->Movement) {
                case Draggable::MovementMode::XAxis:
                    currentPosition.y = currentPosition.z = 0;
                    break;
                case Draggable::MovementMode::YAxis:
                    currentPosition.x = currentPosition.z = 0;
                    break;
                case Draggable::MovementMode::ZAxis:
                    currentPosition.x = currentPosition.y = 0;
                    break;
                
                case Draggable::MovementMode::XYPlane:
                    currentPosition.z = 0;
                    break;
                case Draggable::MovementMode::XZPlane:
                    currentPosition.y = 0;
                    break;
                case Draggable::MovementMode::YZPlane:
                    currentPosition.x = 0;
                    break;
                default:
                    currentPosition = local;
                    break;
            }
                       
            Vector3 position = d.transform->Local().TransformPosition(currentPosition);//localPosition - d.offset + d.transform->Anchor);
            d.transform->Position = position;
        }
        
    }
}

bool DraggableSystem::IsTouchIndexUsed(int touchIndex) {
    if (draggingObjects.empty()) return false;
    for (auto it = draggingObjects.begin(); it!=draggingObjects.end(); ++it) {
        if (it->second.touch.Index == touchIndex) return true;
    }
    return false;
}

void DraggableSystem::Down(TouchData d, GameObject* object) {
    auto it = draggingObjects.find(d.Touchable);
    if (it!=draggingObjects.end()) return;
    DraggingObject& dragging = draggingObjects[d.Touchable];
    dragging.touchable = d.Touchable;
    dragging.transform = object->GetComponent<Transform>();
    dragging.touch = d;
    
    dragging.offset = dragging.transform->WorldInverse().TransformPosition(d.WorldPosition);
    dragging.draggable = object->GetComponent<Draggable>();
    
    Vector3 forward = d.CameraTransform->World().TransformVector(Vector3(0,0,-1));
    
    switch (dragging.draggable->Movement) {
        case  Draggable::MovementMode::XAxis: {
            Vector3 axis = dragging.transform->World().TransformVector(Vector3(1,0,0));
            Vector3 up = axis.Cross(forward);
            dragging.dragPlane = Plane(axis.Cross(up).Normalized(), d.WorldPosition);
        }
        break;
        case  Draggable::MovementMode::YAxis: {
            Vector3 axis = dragging.transform->World().TransformVector(Vector3(0,1,0));
            Vector3 up = axis.Cross(forward);
            dragging.dragPlane = Plane(axis.Cross(up).Normalized(), d.WorldPosition);
        }
        break;
        case  Draggable::MovementMode::ZAxis: {
            bool leftSide = d.CameraTransform->WorldInverse().TransformPosition(d.WorldPosition).x<0;
            
            Vector3 axis = dragging.transform->World().TransformVector(Vector3(0,0,1));
            Vector3 sideways = d.CameraTransform->World().TransformVector(Vector3(leftSide ? -1 : 1,0,0));
            Vector3 up = axis.Cross(sideways);
            dragging.dragPlane = Plane(axis.Cross(up).Normalized(), d.WorldPosition);
        }
        break;
        case  Draggable::MovementMode::XYPlane: {
            Vector3 axis = dragging.transform->World().TransformVector(Vector3(0,0,1));
            dragging.dragPlane = Plane(axis.Normalized(), d.WorldPosition);
        }
        break;
        case  Draggable::MovementMode::XZPlane: {
            Vector3 axis = dragging.transform->World().TransformVector(Vector3(0,1,0));
            dragging.dragPlane = Plane(axis.Normalized(), d.WorldPosition);
        }
        break;
        case  Draggable::MovementMode::YZPlane: {
            Vector3 axis = dragging.transform->World().TransformVector(Vector3(1,0,0));
            dragging.dragPlane = Plane(axis.Normalized(), d.WorldPosition);
        }
        break;
            
        default:{
            dragging.dragPlane = Plane(d.WorldNormal, d.WorldPosition);
        }
        break;
    }
    dragging.draggable->IsDragging = true;
    
}

void DraggableSystem::Up(TouchData d, GameObject* object) {
    auto it = draggingObjects.find(d.Touchable);
    if (it==draggingObjects.end()) return;
    it->second.draggable->IsDragging = false;
    draggingObjects.erase(it);
}
