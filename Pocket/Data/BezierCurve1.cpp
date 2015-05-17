//
//  BezierCurve1.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 4/30/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "BezierCurve1.hpp"
#include "MathHelper.hpp"

using namespace Pocket;

BezierCurve1::BezierCurve1() {}
BezierCurve1::~BezierCurve1() {}

void BezierCurve1::Add(float position, float value, float leftTangent, float rightTangent) {
    Node n;
    n.value = value;
    n.leftTangent = leftTangent;
    n.rightTangent = rightTangent;
    nodes.Add(n, position);
}

float BezierCurve1::Evaluate(float position) {
    if (nodes.Nodes().empty()) return position;
    const NodeLine<Node>::Result& result = nodes.Get(position);
    return MathHelper::Bezier(result.Source.value, result.Source.rightTangent, result.Dest.value, result.Dest.leftTangent, result.t);
}