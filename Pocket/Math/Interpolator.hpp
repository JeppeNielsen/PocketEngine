//
//  Interpolator.h
//  Typhoon
//
//  Created by Jeppe Nielsen on 1/18/12.
//  Copyright (c) 2012 Crowdplayce. All rights reserved.
//

#pragma once//;



namespace Pocket {
    class Interpolator {
    public:
        
        Interpolator();
        ~Interpolator();
        
        Interpolator(float start, float end, float startTangent, float endTangent, float tension, float bias);
        
        
        float Start;
        float End;
        
        float StartTangent;
        float EndTangent;
        
        float Tension;
        float Bias;
        
        
        float Interpolate(const float& value);
        
        static Interpolator Ease();
        static Interpolator EaseIn();
        static Interpolator EaseOut();
        
        
    };
}


