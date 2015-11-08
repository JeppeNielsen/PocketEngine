//
//  CameraDragSystem.cpp
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 07/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "CameraDragSystem.h"
#include "Plane.hpp"
#include "MathHelper.hpp"

CameraDragSystem::CameraDragSystem() : Input(this) {
    Input = 0;
    Input.ChangedWithOld += event_handler(this, &CameraDragSystem::InputChanged);
}

void CameraDragSystem::InputChanged(Property<CameraDragSystem *, InputManager *>::EventData e) {
    if (e.Old) {
        e.Old->TouchDown -= event_handler(this, &CameraDragSystem::TouchDown);
        e.Old->TouchUp -= event_handler(this, &CameraDragSystem::TouchUp);
        e.Current->ScrollChanged -= event_handler(this, &CameraDragSystem::ScrollChanged);
    }
    
    if (e.Current) {
        e.Current->TouchDown += event_handler(this, &CameraDragSystem::TouchDown);
        e.Current->TouchUp += event_handler(this, &CameraDragSystem::TouchUp);
        e.Current->ScrollChanged += event_handler(this, &CameraDragSystem::ScrollChanged);
    }
}

void CameraDragSystem::TouchDown(Pocket::TouchEvent e) {
    if (e.Index!=1) return;
    Plane plane({0,0,1},0);
    for (GameObject* go : Objects()) {
        Ray ray = go->GetComponent<Camera>()->GetRay(go->GetComponent<Transform>(), Input()->GetTouchPosition(e.Index));

        float dist;
        if (plane.IntersectsRay(ray, &dist)) {
            go->GetComponent<CameraDragger>()->startDragPosition = ray.GetPosition(dist);
            go->GetComponent<CameraDragger>()->startTransformPosition = go->GetComponent<Transform>()->Position;
            go->GetComponent<CameraDragger>()->isDragging = true;
        }
    }
}

void CameraDragSystem::TouchUp(Pocket::TouchEvent e) {
    if (e.Index!=1) return;
    for (GameObject* go : Objects()) {
        go->GetComponent<CameraDragger>()->isDragging = false;
    }
}

void CameraDragSystem::Update(float dt) {
    Plane plane({0,0,1},0);
    for (GameObject* go : Objects()) {
        CameraDragger* dragger = go->GetComponent<CameraDragger>();
        if (!dragger->isDragging) continue;
        
        go->GetComponent<Transform>()->Position = go->GetComponent<CameraDragger>()->startTransformPosition;
        Ray ray = go->GetComponent<Camera>()->GetRay(go->GetComponent<Transform>(), Input()->GetTouchPosition(1));

        float dist;
        if (plane.IntersectsRay(ray, &dist)) {
            Vector3 offset = go->GetComponent<CameraDragger>()->startDragPosition - ray.GetPosition(dist);
            go->GetComponent<Transform>()->Position = go->GetComponent<CameraDragger>()->startTransformPosition + offset;
        }
    }
    
    //vector2f projected(const vector2f &v)
	//{   return v * (*this * v)/(v*v);	}

    
    for (GameObject* go : Objects()) {
        CameraDragger* dragger = go->GetComponent<CameraDragger>();
        Vector3 pos = go->GetComponent<Transform>()->Position;
        
        float delta = (dragger->zPos - pos.z) * dt * 20.0f;
        
        Ray ray = go->GetComponent<Camera>()->GetRay(go->GetComponent<Transform>(), Input()->GetTouchPosition(1));

        // ray.direction;
        
        Vector3 forward(0,0,delta);
        Vector3 xyDelta = (forward * ray.direction.Dot(forward));
        Vector3 projected = ray.direction * (forward.Dot(ray.direction)) / ray.direction.Dot(ray.direction);
        
        Plane plane({0,0,1}, ray.position+ Vector3(0,0,delta));
        float dist;
        plane.IntersectsRay(ray, &dist);
        projected = ray.direction * dist;

        pos.z += delta;

        pos.x += projected.x;// * delta;
        pos.y += projected.y;// * delta;
    
        
        
        
        
        /*
        
        float zDelta = dragger->zPos - dragger->zPosPrev;
        //if (zDelta<0) zDelta=-zDelta;
        
        if (zDelta>0.0001f || zDelta<-0.0001f) {
            float dT = (pos.z - dragger->zPosPrev) / zDelta;
            Vector3 scrollPos = dragger->scrollLocation + dragger->scrollVector * dT;
            pos.x = scrollPos.x;
            pos.y = scrollPos.y;
        }
        */
        go->GetComponent<Transform>()->Position = pos;
    }
}

void CameraDragSystem::ScrollChanged(float delta) {
    
    for (GameObject* go : Objects()) {
        CameraDragger* dragger = go->GetComponent<CameraDragger>();
        dragger->zPosPrev = dragger->zPos;
        dragger->zPos -= delta;
        dragger->zPos = MathHelper::Clamp(dragger->zPos, 2, 100);
        
        Plane plane({0,0,1},dragger->zPos);
        Ray ray = go->GetComponent<Camera>()->GetRay(go->GetComponent<Transform>(), Input()->GetTouchPosition(1));

        float dist;
        if (plane.IntersectsRay(ray, &dist)) {
            dragger->scrollLocation = go->GetComponent<Transform>()->Position;
            dragger->scrollVector = ray.GetPosition(dist) - dragger->scrollLocation;
        }
    }
}
