//
//  MovableCollisionSystem.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 12/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "MovableCollisionSystem.h"

void MovableCollisionSystem::Update(float dt) {
    
    /*
    for (auto o : Objects()) {
        Transform* t = o->GetComponent<Transform>();
        Movable* m = o->GetComponent<Movable>();
        Mappable* mappable = o->GetComponent<Mappable>();
        Vector3 normal;
        if (IsPositionValid(t->Position, mappable->Map, normal)) {
            m->prevPosition = t->Position;
        }
    }
    */
    
    
    for (int iteration = 0; iteration<2; iteration++) {
    
    for (int i=0; i<Objects().size(); i++) {
        for (int j=i+1; j<Objects().size(); j++) {
            
            Mappable* mappable = Objects()[i]->GetComponent<Mappable>();
            Transform* t1 = Objects()[i]->GetComponent<Transform>();
            Transform* t2 = Objects()[j]->GetComponent<Transform>();
            Vector3 p1 = t1->Position;
            Vector3 p2 = t2->Position;
            Vector3 vector = p2-p1;
            vector.y = 0;
            vector.x += 0.001f;
            
            float distance = vector.LengthSqaured();
            
            if (distance<4.0f) {
                distance = sqrtf(distance);
                //vector /= distance;
                
                
                p1 -= vector * 2.0f * dt;
                p2 += vector * 2.0f * dt;
                
                /*
                Vector3 normal;
                if (!IsPositionValid(p1, mappable->Map, normal)) {
                    p1 += normal*dt*3;
                }
                
                if (!IsPositionValid(p2, mappable->Map, normal)) {
                    p2 += normal*dt*3;
                }
                */
                
                t1->Position = p1;
                t2->Position = p2;

                
               
            }
            
        }
    }
    }
/*
    for (auto o : Objects()) {
        Transform* t = o->GetComponent<Transform>();
        Movable* m = o->GetComponent<Movable>();
        Mappable* mappable = o->GetComponent<Mappable>();
        
        Vector3 normal;
        if (!IsPositionValid(t->Position, mappable->Map, normal)) {
            t->Position = (m->prevPosition);// - t->Position) * 4.0f * dt;
        }
    }
    */
}

bool MovableCollisionSystem::IsPositionValid(Pocket::Vector3 p, Map* map, Vector3& collisionNormal) {
    int x = (int)floorf(p.x);
    int z = (int)floorf(p.z);
    const Map::Node& node = map->GetNode(x,z);
    bool valid = node.height>0.48f;
    
    if (valid) return true;
    
    static const Point directions[] = {
        {1,0}, {1,1}, {0,1}, {-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}
    };
    
    for (int i=0; i<8; i++) {
        Point dir = directions[i];
        const Map::Node& neighbor = map->GetNode(x + dir.x, z + dir.y);
        if (neighbor.height>0.48f) {
            
            collisionNormal = Vector3((float)x + dir.x, 0, (float)z + dir.y) - p;
            collisionNormal.Normalize();
            break;
        }
    }
    
    return false;
}
