//
//  BezierCurve3.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/26/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//


#pragma once
#include "NodeLine.hpp"
#include "Vector3.hpp"
#include "Matrix4x4.hpp"


namespace Pocket {
    class BezierCurve3 {
    public:
        BezierCurve3();
        ~BezierCurve3();
        
        void Add(float position, const Vector3& value);
        void Add(float position, const Vector3& value, const Vector3& leftTangent, const Vector3& rightTangent);
        
        Vector3 Evaluate(float position);
        
        Matrix4x4 GetWorld(float position, const Vector3& up);
                
        struct Node {
            Vector3 value;
            Vector3 leftTangent;
            Vector3 rightTangent;
        };
        
        NodeLine<Node> nodes;
    };
}