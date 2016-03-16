//
//  RigidBody2d.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 12/28/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "b2Body.h"
#include "b2PolygonShape.h"
#include "b2CircleShape.h"
#include <vector>
#include "Event.hpp"

namespace Pocket {
    class RigidBody2d {
    public:

        RigidBody2d();

        void Reset();

        b2BodyDef bodyDefinition;

        typedef std::vector<b2PolygonDef> PolygonList;
        typedef std::vector<b2CircleDef> CircleList;

        PolygonList& Polygons();
        CircleList& Circles();

        const PolygonList& ReadOnlyPolygons() const;
        const CircleList& ReadOnlyCircles() const;

        b2Body* body;

        b2PolygonDef& CreatePolygon(float density = 0.0f, float friction = 1.0f, float restitution = 0.0f);
        b2CircleDef& CreateCircle(float density = 0.0f, float friction = 1.0f, float restitution = 0.0f);

        Event<RigidBody2d*> HasBecomeDirty;

    private:
        PolygonList polygons;
        CircleList circles;
    };
}