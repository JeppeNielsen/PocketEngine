//
//  BezierCurve1.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 4/30/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "NodeLine.hpp"


namespace Pocket {
    class BezierCurve1 {
    public:
        BezierCurve1();
        ~BezierCurve1();
        
        void Add(float position, float value, float leftTangent, float rightTangent);
        
        float Evaluate(float position);
        
        struct Node {
            float value;
            float leftTangent;
            float rightTangent;
        };
        
        NodeLine<Node> nodes;
    };
}