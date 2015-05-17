//
//  TransitionHelper.h
//  FlingTheFishX
//
//  Created by Jeppe Nielsen on 8/5/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#pragma once//;
namespace Pocket {
    class TransitionHelper {
    public:
        static float easeOutElastic(float start, float end, float value);
        static float easeInElastic(float start, float end, float value);
        static float punch(float amplitude, float value);
        static float CalcAngle(float Ang1, float Ang2);
        static float HermiteInterpolate(float mu, float y1, float y2, float y0, float y3, float tension, float bias);     
    };
}