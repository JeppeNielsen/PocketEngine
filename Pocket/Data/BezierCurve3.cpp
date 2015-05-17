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

BezierCurve3::BezierCurve3() { nodes.Extrapolate = true; }
BezierCurve3::~BezierCurve3() {}

void BezierCurve3::Add(float position, const Vector3& value) {
    Add(position, value, value, value);
}

void BezierCurve3::Add(float position, const Vector3& value, const Vector3& leftTangent, const Vector3& rightTangent) {
    Node n;
    n.value = value;
    n.leftTangent = leftTangent;
    n.rightTangent = rightTangent;
    nodes.Add(n, position);
}

Vector3 BezierCurve3::Evaluate(float position) {
    if (nodes.Nodes().empty()) return position;
    const NodeLine<Node>::Result& result = nodes.Get(position);
    return Vector3(
                   MathHelper::Bezier(result.Source.value.x, result.Source.rightTangent.x, result.Dest.value.x, result.Dest.leftTangent.x, result.t),
                   MathHelper::Bezier(result.Source.value.y, result.Source.rightTangent.y, result.Dest.value.y, result.Dest.leftTangent.y, result.t),
                   MathHelper::Bezier(result.Source.value.z, result.Source.rightTangent.z, result.Dest.value.z, result.Dest.leftTangent.z, result.t)
                   );
}

Matrix4x4 BezierCurve3::GetWorld(float position, const Vector3& up) {
    Vector3 start = Evaluate(position);
    Vector3 end = Evaluate(position + 0.01f);
    Matrix4x4 m;
    m.InitTransform(start, 1.0f, Quaternion::LookAt(start, end, up));
    return m;
}