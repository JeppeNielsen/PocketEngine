//
//  SpringColisionSystem.cpp
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/7/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "SpringCollisionSystem.h"
#include <iostream>

#define MIN(X,Y) ((X<Y) ? (X) : (Y))
#define MAX(X,Y) ((X>Y) ? (X) : (Y))

void Body::ProjectToAxis(const Vector2& axis, float& min, float& max ) {
  float dot = axis.Dot(particles[0]->position);
  
  //Set the minimum and maximum values to the projection of the first vertex
  min = max = dot;

  for( int i = 1; i < 3; i++ ) {
    //Project the rest of the vertices onto the axis and extend 
    //the interval to the left/right if necessary
    dot = axis.Dot(particles[i]->position);

    min = MIN( dot, min );
    max = MAX( dot, max );
  } 
}

bool Body::SpringNotIntersects(Spring* spring) {

    Vector2 minSpring = spring->particleA->position.Min(spring->particleB->position);
    Vector2 maxSpring = spring->particleA->position.Max(spring->particleB->position);
    
    Vector2 min;
    Vector2 max;
    min = max = particles[0]->position;
    for (int i=1; i<3; i++) {
        min = particles[i]->position.Min(min);
        max = particles[i]->position.Max(max);
    }
    
    
    return (minSpring.x > max.x
                || maxSpring.x < min.x
                || maxSpring.y < min.y
                || minSpring.y > max.y);
}

void Body::CalcMinMax(Vector2& min, Vector2& max) {
    min = max = particles[0]->position;
    for (int i=1; i<3; i++) {
        min = particles[i]->position.Min(min);
        max = particles[i]->position.Max(max);
    }
}

/*
bool Box::Intersect(const Box& other) const{
	return !(left > other.right
                || right < other.left
                || top > other.bottom
                || bottom < other.top);
}
*/


void SpringCollisionSystem::Initialize() {
    AddComponent<Spring>();
    bodiesNeedsCalculating = false;
    quadtree.SetMinMax({-130,-140}, {130,140});
    UseTree = true;
}

void SpringCollisionSystem::ObjectAdded(GameObject* object) {
    
    bodiesNeedsCalculating=true;
}

void SpringCollisionSystem::CreateBodyFromSpring(Spring* spring) {

    Springs connectedToA;
    FindSpringsConnectedToParticle(spring->particleA, spring, connectedToA);
    if (connectedToA.empty()) return; //no body can be connnected since both A and B needs to be connected
    
    Springs connectedToB;
    FindSpringsConnectedToParticle(spring->particleB, spring, connectedToB);
    if (connectedToB.empty()) return; //no body can be connnected since both A and B needs to be connected
    
    Spring* foundA = 0;
    Spring* foundB = 0;
    
    for (int a=0; a<connectedToA.size(); a++) {
        Spring* springA = connectedToA[a];
        for (int b=0; b<connectedToB.size(); b++) {
            Spring* springB = connectedToB[b];
            
            if (springA->particleA == springB->particleA || springA->particleA == springB->particleB ||
                springA->particleB == springB->particleA || springA->particleB == springB->particleB) {
                
                foundA = springA;
                foundB = springB;
                
            }
            if (foundB) break;
        }
        if (foundA) break;
    }
    
    if (foundA && foundB) {
    
        Particle* p1 = spring->particleA;
        Particle* p2 = spring->particleB;
        Particle* p3 = foundA->particleA == spring->particleA ? foundA->particleB : foundA->particleA;
        
        if (BodyExists(p1,p2,p3)) return;
        
        Body* body = new Body();
        
        bodies.push_back(body);
        
        body->particles[0]=p1;
        body->particles[1]=p2;
        body->particles[2]=p3;
        body->CalcMinMax(body->node.min, body->node.max);
        body->node.data = body;
        quadtree.Insert(body->node);
    }
}

void SpringCollisionSystem::FindSpringsConnectedToParticle(Particle* particle, Spring* excludingSpring, Springs& springs) {
    for (int i=0; i<Objects().size(); i++) {
        Spring* spring = Objects()[i]->GetComponent<Spring>();
        if (spring == excludingSpring) continue;
        if (spring->particleA == particle || spring->particleB == particle) {
            springs.push_back(spring);
        }
    }
}

bool SpringCollisionSystem::BodyExists(Particle* p1, Particle* p2, Particle* p3) {
    for (int i=0; i<bodies.size(); i++) {
        Body* body = bodies[i];
        if (body->particles[0]!=p1 && body->particles[1]!=p1 && body->particles[2]!=p1) continue;
        if (body->particles[0]!=p2 && body->particles[1]!=p2 && body->particles[2]!=p2) continue;
        if (body->particles[0]!=p3 && body->particles[1]!=p3 && body->particles[2]!=p3) continue;
        return true;
    }
    return false;
}

void SpringCollisionSystem::Simulate(float dt) {

    if (bodiesNeedsCalculating) {
        for (int i=0; i<bodies.size(); i++) {
            if (bodies[i]->node.node) {
                quadtree.Remove(bodies[i]->node);
            }
            delete bodies[i];
        }
        bodies.clear();
        bodiesNeedsCalculating = false;
        for (int i=0; i<Objects().size(); i++) {
            CreateBodyFromSpring(Objects()[i]->GetComponent<Spring>());
        }
    }


    /*
    Body body1;
    Body body2;
    
    
    
    
    
    body1.edges[0] = Objects()[0]->GetComponent<Spring>();
    body1.edges[1] = Objects()[1]->GetComponent<Spring>();
    body1.edges[2] = Objects()[2]->GetComponent<Spring>();
    
    body2.edges[0] = Objects()[3]->GetComponent<Spring>();
    body2.edges[1] = Objects()[4]->GetComponent<Spring>();
    body2.edges[2] = Objects()[5]->GetComponent<Spring>();
    */
      
    for (int i=0; i<bodies.size(); i++) {
        Body* body = bodies[i];
        body->CalcMinMax(body->node.min, body->node.max);
        if (body->node.node) {
            quadtree.Move(body->node);
        } else {
            quadtree.Insert(body->node);
        }
    }
    
    int comparisons = 0;
    
    if (UseTree) {
        
        Vector2 min;
        Vector2 max;
        CollisionInfo info;
        info.particle = 0;
        info.springA = 0;
        info.springB = 0;
        for (int i=0; i<Objects().size(); i++) {
            Spring* spring = Objects()[i]->GetComponent<Spring>();
            if (!spring->particleA) continue;
            if (!spring->particleB) continue;
            
            min = spring->particleA->position.Min(spring->particleB->position);
            max = spring->particleA->position.Max(spring->particleB->position);
            bodiesInQuery.clear();
            quadtree.Get<Body*>(min, max, bodiesInQuery);
            
            //std::cout << " Bodies: " << bodies.size() << "  bodiesInQuery :"<<bodiesInQuery.size()<<std::endl;
            
            for (int j=0; j<bodiesInQuery.size(); j++) {
                comparisons++;
                if (DetectCollision(bodiesInQuery[j], spring, info)) {
                    CollisionResponse(info);
                }
            }
        }
    
    } else {
    
        CollisionInfo info;
        for (int i=0; i<Objects().size(); i++) {
            Spring* spring = Objects()[i]->GetComponent<Spring>();
            
            for (int i=0; i<bodies.size(); i++) {
                comparisons++;
                if (DetectCollision(bodies[i], spring, info)) {
                    CollisionResponse(info);
                }
            }
        }
    
    }
    
    //static int counter = 0;
    //if (counter++ % 20 == 0) {
    //    std::cout<<"Comparisons :" << comparisons << std::endl;
    //}
}

void SpringCollisionSystem::CollisionResponse(CollisionInfo& collisionInfo) {

    Vector2 collisionVector = collisionInfo.normal*collisionInfo.depth;
    if (collisionInfo.depth<=0.00001f) return;

    Particle* E1 = collisionInfo.springA;
    Particle* E2 = collisionInfo.springB;

    float dx = E1->position.x - E2->position.x;
    float dy = E1->position.y - E2->position.y;
    
    if (dx<0) dx=-dx;
    if (dy<0) dy=-dy;

    float T;
    
    if( dx > dy ) {
        T = ( collisionInfo.particle->position.x - collisionVector.x - E1->position.x )/(  E2->position.x - E1->position.x);
    } else {
        T = ( collisionInfo.particle->position.y - collisionVector.y - E1->position.y )/(  E2->position.y - E1->position.y);
    }
    float oneMinusT = 1 - T;
    
    float Lambda = 1.0f/( T*T + oneMinusT*oneMinusT );

    Vector2 tangent = Vector2(-collisionInfo.normal.y, collisionInfo.normal.x);
    
    Vector2 velA = E1->position - E1->positionOld;
    Vector2 velB = E2->position - E2->positionOld;
    
    Vector2 velAB = velA + (velB - velA) * 0.5f;
    
    Vector2 velP = collisionInfo.particle->position - collisionInfo.particle->positionOld;
    
    Vector2 relativeVelocity = velP - velAB;
    
    const float friction = 0.5f;
    
    if (!E1->immovable) {
        Vector2 velocity = relativeVelocity * oneMinusT * Lambda;
        float dot = tangent.Dot(velocity);
        Vector2 frictionImpulse = tangent * dot * friction;
        E1->position -= collisionVector * oneMinusT * 0.5f * Lambda - frictionImpulse * 0.5f;
    }
    
    if (!E2->immovable) {
        Vector2 velocity = relativeVelocity * T * Lambda;
        float dot = tangent.Dot(velocity);
        Vector2 frictionImpulse = tangent * dot * friction;
        E2->position -= collisionVector * T * 0.5f * Lambda - frictionImpulse * 0.5f;
    }
    
    if (!collisionInfo.particle->immovable) {
        float dot = tangent.Dot(relativeVelocity);
        Vector2 frictionImpulse = tangent * dot * friction;
        collisionInfo.particle->position += collisionVector * 0.5f - frictionImpulse * 0.5f;
    }
}

float SpringCollisionSystem::IntervalDistance( float MinA, float MaxA, float MinB, float MaxB ) {
  if( MinA < MinB )
    return MaxA - MinB;
  else
    return MaxB - MinA;
}

bool SpringCollisionSystem::DetectCollision(Body* body, Spring* spring, CollisionInfo& collisionInfo) {
    
    if ((spring->particleA == body->particles[0] || spring->particleA == body->particles[1] || spring->particleA == body->particles[2])
        &&
        (spring->particleB == body->particles[0] || spring->particleB == body->particles[1] || spring->particleB == body->particles[2])) {
        return false;
    }
    
    //if (body->SpringNotIntersects(spring)) return false;
    
    
    /*
    int count = 0;
    for (int i=0; i<3; i++) {
        if (spring->particleA == body->particles[i]) count++;
        if (spring->particleB == body->particles[i]) count++;
        if (count==2) return false;
    }
    */
    float minDistance = 100000.0f;

    Vector2 bodyCenter = body->particles[0]->position + body->particles[1]->position + body->particles[2]->position;
    bodyCenter*=0.33333333f;
    
    bool collisionEdgeIsSpring = false;
    
    for (int i=0; i<4; i++) {
        Particle* pa;
        Particle* pb;
        if (i<2) {
            pa = body->particles[i];
            pb = body->particles[i + 1];
        } else if (i<3) {
            pa = body->particles[2];
            pb = body->particles[0];
        } else {
            pa = spring->particleA;
            pb = spring->particleB;
        }
        
        Vector2 axis(pa->position.y - pb->position.y, pb->position.x - pa->position.x);

        if (i==3) {
            float dot = axis.Dot(spring->particleA->position - bodyCenter);
            if (dot<0) axis =-axis;
        }
        
        axis.Normalize();
        
        float minA, minB, maxA, maxB;
        body->ProjectToAxis(axis, minA, maxA);
        spring->ProjectToAxis(axis, minB, maxB);
    
        float distance = IntervalDistance( minA, maxA, minB, maxB );

        if (distance < 0.0f) return false;
        
        if (distance<minDistance) {
            minDistance = distance;
            collisionInfo.normal = axis;
            collisionInfo.springA = pa;
            collisionInfo.springB = pb;
            collisionEdgeIsSpring = i==3;
        }
    }
        
    collisionInfo.depth = minDistance;
    
    Vector2 springCenter = (spring->particleA->position + spring->particleB->position) * 0.5f;
    
    if (collisionEdgeIsSpring) {
    
        float dot = collisionInfo.normal.Dot(bodyCenter - springCenter);
        if (dot<=0) collisionInfo.normal = -collisionInfo.normal;
    
        float smallestDistance = 10000.0f;
        for (int i=0; i<3; i++) {
            float d = collisionInfo.normal.Dot(body->particles[i]->position - springCenter);
            if (d<smallestDistance) {
                smallestDistance = d;
                collisionInfo.particle = body->particles[i];
            }
        }
    } else {
        float dot = collisionInfo.normal.Dot(springCenter-bodyCenter);
        if (dot<=0) collisionInfo.normal = -collisionInfo.normal;
    
        float d1 = collisionInfo.normal.Dot(spring->particleA->position - bodyCenter);
        float d2 = collisionInfo.normal.Dot(spring->particleB->position - bodyCenter);
        collisionInfo.particle = d1<d2 ? spring->particleA : spring->particleB;
    }
    
    return true;
}







