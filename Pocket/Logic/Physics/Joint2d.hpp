//
//  Joint2d.hpp
//  MotoCross
//
//  Created by Jeppe Nielsen on 2/6/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Vector2.hpp"
#include "RigidBody2d.hpp"
#include "Property.hpp"
#include "b2DistanceJoint.h"
#include "b2PrismaticJoint.h"
#include "b2RevoluteJoint.h"

namespace Pocket {
    class Joint2d {
    public:
        Joint2d();
        ~Joint2d();
    
        b2JointDef* jointDef;
        b2Joint* joint;

        template<typename T>
        T& Joint() {
            T* j = (T*)joint;
            return *j;
        }

        Property<RigidBody2d*> BodyA;
        Property<RigidBody2d*> BodyB;

        b2DistanceJointDef& CreateDistance(GameObject* a, GameObject* b, float length, const Vector2& localAnchorA, const Vector2& localAnchorB);
        b2PrismaticJointDef& CreateSlider(GameObject* a, GameObject* b, const Vector2& localAnchorA, const Vector2& localAnchorB, const Vector2& localAxisA, float referenceAngle, float minDistance = 0, float maxDistance = 0);

        b2RevoluteJointDef& CreatePin(GameObject* a, GameObject*b, const Vector2& localAnchorA, const Vector2& localAnchorB, float referenceAngle, bool enableLimit = false, float lowerAngle = 0, float upperAngle = 0, bool enableMotor = false, float motorSpeed = 0, float maxMotorTorque = 0);

    };
}