//
//  BezierCurve3.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/26/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "BezierCurve3.hpp"
#include "MathHelper.hpp"
#include "Quaternion.hpp"

using namespace Pocket;

BezierCurve3::BezierCurve3() { }
BezierCurve3::~BezierCurve3() { }

void BezierCurve3::Add(float position, const Vector3& value) {
    Add(position, value, value, value);
}

void BezierCurve3::Add(float position, const Vector3& value, const Vector3& leftTangent, const Vector3& rightTangent) {
    nodes.AddNode(position, {
        value,
        leftTangent,
        rightTangent
    });
}

Vector3 BezierCurve3::Evaluate(float position) {
    if (nodes.Count() == 0) {
        return Vector3(0,0,0);
    }
    
    Vector3 result;
    nodes.Evaluate(position, [&result](float t, const Node& a, const Node&b) {
        result.x = MathHelper::Bezier(a.value.x, a.rightTangent.x, b.value.x, b.leftTangent.x, t);
        result.y = MathHelper::Bezier(a.value.y, a.rightTangent.y, b.value.y, b.leftTangent.y, t);
        result.z = MathHelper::Bezier(a.value.z, a.rightTangent.z, b.value.z, b.leftTangent.z, t);
    });

    return result;
}

Matrix4x4 BezierCurve3::GetWorld(float position, const Vector3& up) {
    Vector3 start = Evaluate(position);
    Vector3 end = Evaluate(position + 0.01f);
    Matrix4x4 m;
    m.InitTransform(start, 1.0f, Quaternion::LookAt(start, end, up));
    return m;
}
