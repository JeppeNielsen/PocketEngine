//
//  SpringColisionSystem.h
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/7/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//


#pragma once
#include "GameWorld.hpp"
#include "Spring.h"
#include "Particle.h"
#include <vector>
#include "Quadtree.hpp"
using namespace Pocket;

class Body {
public:
    Particle* particles[3];
    void ProjectToAxis(const Vector2& axis, float& min, float& max );
    bool SpringNotIntersects(Spring* spring);
    void CalcMinMax(Vector2& min, Vector2& max);
    Quadtree::Node node;
};

struct CollisionInfo {
    float depth;
    Vector2 normal;
    Particle* springA;
    Particle* springB;
    Particle* particle;
};

class SpringCollisionSystem : public GameSystem {
public:
    void Initialize();
    void Simulate(float dt);
    
    void ObjectAdded(GameObject* object);
    
    bool UseTree;

private:
    float IntervalDistance( float MinA, float MaxA, float MinB, float MaxB );
    bool DetectCollision(Body* body, Spring* spring, CollisionInfo& collisionInfo);
    void CollisionResponse(CollisionInfo& collisionInfo);

    void CreateBodyFromSpring(Spring* spring);
    
    typedef std::vector<Spring*> Springs;
    
    void FindSpringsConnectedToParticle(Particle* particle, Spring* excludingSpring, Springs& springs);
    
    
    void CalculateBodies();
    bool BodyExists(Particle* p1, Particle* p2, Particle* p3);
    
    
    bool bodiesNeedsCalculating;
    typedef std::vector<Body*> Bodies;
    Bodies bodies;
    Bodies bodiesInQuery;
    
    Quadtree quadtree;
};