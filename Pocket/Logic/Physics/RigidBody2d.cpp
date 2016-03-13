//
//  RigidBody2d.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 12/28/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "RigidBody2d.hpp"

using namespace Pocket;

RigidBody2d::RigidBody2d() : body(0) {
    bodyDefinition.linearDamping = 1.6f;
    bodyDefinition.angularDamping = 0.1f;
    polygons.clear();
    circles.clear();
    body = 0;
}

b2PolygonDef& RigidBody2d::CreatePolygon(float density, float friction, float restitution) {
    Polygons().resize(polygons.size() + 1);
    b2PolygonDef& def = polygons[polygons.size() - 1];
    def.density = density;
    def.friction = friction;
    def.restitution = restitution;
    return def;
}

b2CircleDef& RigidBody2d::CreateCircle(float density, float friction, float restitution) {
    Circles().resize(circles.size() + 1);
    b2CircleDef& def = circles[circles.size() - 1];
    def.density = density;
    def.friction = friction;
    def.restitution = restitution;
    return def;
}

RigidBody2d::PolygonList& RigidBody2d::Polygons() {
    HasBecomeDirty(this);
    return polygons;
}

RigidBody2d::CircleList& RigidBody2d::Circles() {
    HasBecomeDirty(this);
    return circles;
}

const RigidBody2d::PolygonList& RigidBody2d::ReadOnlyPolygons() const {
    return polygons;
}

const RigidBody2d::CircleList& RigidBody2d::ReadOnlyCircles() const {
    return circles;
}
