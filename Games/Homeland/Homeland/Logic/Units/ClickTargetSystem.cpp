//
//  ClickDestinationSystem.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 11/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "ClickTargetSystem.h"
#include "Particle.h"
#include "Mappable.h"

void ClickTargetSystem::Initialize() {
    selectables = world->CreateSystem<Selectables>();
}

void ClickTargetSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Click.Bind(this, &ClickTargetSystem::ObjectClicked);
}

void ClickTargetSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Click.Unbind(this, &ClickTargetSystem::ObjectClicked);
}

void ClickTargetSystem::ObjectClicked(Pocket::TouchData d) {
    if (d.Index != 1) return;
    
    std::vector<GameObject*> objectsToMove;
    for(auto o : selectables->Objects()) {
        if (!o->GetComponent<Selectable>()->Selected()) continue;
        objectsToMove.push_back(o);
       // o->GetComponent<Movable>()->Target = {d.WorldPosition.x, d.WorldPosition.z };
    }
    if (objectsToMove.empty()) return;
    
    Vector2 target = { d.WorldPosition.x, d.WorldPosition.z };
    Vector2 centerPoint = 0;
    for(GameObject* go : objectsToMove) {
        centerPoint += go->GetComponent<Particle>()->position;
    }
    centerPoint *= (1.0f / objectsToMove.size());
    std::vector<Vector2> targets;
    for(GameObject* go : objectsToMove) {
        Vector2 fromCenterPoint = go->GetComponent<Particle>()->position - centerPoint;
        targets.push_back(target + fromCenterPoint.Normalized());
    }
    
    
    for (int iterations=0; iterations<10; iterations++) {
    
        for (int i=0; i<objectsToMove.size(); i++) {
            //Particle* a = objectsToMove[i]->GetComponent<Particle>();
            Vector2& targetA = targets[i];
        
            for (int j=i+1; j<objectsToMove.size(); j++) {
                //Particle* b = objectsToMove[i]->GetComponent<Particle>();
                Vector2& targetB = targets[j];
                const float radius = 2.0f;
                
                Vector2 vector = targetB - targetA;
                float length = vector.Length();
                if (length<radius) {
                    vector *= (1.0f / length);
                    float penetration = radius - length;
                    targetA -= vector * penetration * 0.5f;
                    targetB += vector * penetration * 0.5f;
                }
            }
        }
        /*
        for (int i=0; i<objectsToMove.size(); i++) {
            targets[i] = objectsToMove[i]->GetComponent<Mappable>()->Map->FindNearestValidPosition(targets[i]);
        }
        */
    }
    
    for (int i=0; i<objectsToMove.size(); i++) {
        objectsToMove[i]->GetComponent<Movable>()->Target = targets[i];
    }
}