//
//  Joint2d.cpp
//  MotoCross
//
//  Created by Jeppe Nielsen on 2/6/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "Joint2d.hpp"
#include "GameObject.hpp"

using namespace Pocket;

Joint2d::~Joint2d() {
    delete jointDef;
}

Joint2d::Joint2d() : joint(0), jointDef(0) {

    BodyA = 0;
    BodyB = 0;
    delete jointDef;
    jointDef = 0;
    joint = 0;
}

b2DistanceJointDef& Joint2d::CreateDistance(GameObject* a, GameObject* b, float length, const Vector2 &localAnchorA, const Vector2 &localAnchorB) {
    
    BodyA = a->GetComponent<RigidBody2d>();
    BodyB = b->GetComponent<RigidBody2d>();
    
    delete jointDef;
    b2DistanceJointDef* distanceJoint = new b2DistanceJointDef();
    distanceJoint->length = length;
    distanceJoint->localAnchor1.Set(localAnchorA.x, localAnchorA.y);
    distanceJoint->localAnchor2.Set(localAnchorB.x, localAnchorB.y);
    distanceJoint->dampingRatio = 0.0f;
    distanceJoint->frequencyHz = 4;
    jointDef = distanceJoint;
    return *distanceJoint;
}

b2PrismaticJointDef& Joint2d::CreateSlider(GameObject *a, GameObject *b, const Vector2 &localAnchorA, const Vector2 &localAnchorB, const Vector2 &localAxisA, float referenceAngle, float minDistance, float maxDistance) {
    delete jointDef;
    
    BodyA = a->GetComponent<RigidBody2d>();
    BodyB = b->GetComponent<RigidBody2d>();
    
    b2PrismaticJointDef* sliderJoint = new b2PrismaticJointDef();
    sliderJoint->localAnchor1.Set(localAnchorA.x, localAnchorA.y);
    sliderJoint->localAnchor2.Set(localAnchorB.x, localAnchorB.y);
    Vector2 axis = localAxisA.Normalized();
    sliderJoint->localAxis1.Set(axis.x, axis.y);
    sliderJoint->referenceAngle = referenceAngle;
    sliderJoint->enableLimit = minDistance!=0 || maxDistance!=0;
    sliderJoint->lowerTranslation = minDistance;
    sliderJoint->upperTranslation = maxDistance;
    jointDef = sliderJoint;
    return *sliderJoint;
}

b2RevoluteJointDef& Joint2d::CreatePin(GameObject *a, GameObject *b,
                                       const Vector2 &localAnchorA, const Vector2 &localAnchorB, float referenceAngle,
                                       bool enableLimit, float lowerAngle, float upperAngle,
                                       bool enableMotor, float motorSpeed, float maxMotorTorque) {
    delete jointDef;
    
    BodyA = a->GetComponent<RigidBody2d>();
    BodyB = b->GetComponent<RigidBody2d>();
    
    b2RevoluteJointDef* pinJoint = new b2RevoluteJointDef();
    pinJoint->localAnchor1.Set(localAnchorA.x, localAnchorA.y);
    pinJoint->localAnchor2.Set(localAnchorB.x, localAnchorB.y);
    pinJoint->referenceAngle = referenceAngle;
    pinJoint->enableMotor = enableMotor;
    pinJoint->lowerAngle = lowerAngle;
    pinJoint->upperAngle = upperAngle;
    pinJoint->enableMotor = enableMotor;
    pinJoint->motorSpeed = motorSpeed;
    pinJoint->maxMotorTorque = maxMotorTorque;
    jointDef = pinJoint;
    return *pinJoint;
}