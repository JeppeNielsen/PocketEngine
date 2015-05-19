//
//  TransformAnimation.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/8/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "BezierCurve3.hpp"

namespace Pocket {
    enum class TransformAnimationPlayMode {
        Clamp,
        Wrap,
        PingPong
    };
    
    class TransformAnimation {
    public:
        TransformAnimation();
        ~TransformAnimation();
        
        BezierCurve3 position;
        BezierCurve3 rotation;
        BezierCurve3 scale;
        TransformAnimationPlayMode positionPlayMode;
        TransformAnimationPlayMode rotationPlayMode;
        TransformAnimationPlayMode scalePlayMode;
        
        void Clear();
    
        void GetValues(Vector3* values, float* progress);
        
        void GetPercentageProgresses(float* progress, float percentage);
        
    private:
        
        float GetProgress(float progress, float duration, TransformAnimationPlayMode playMode);
        
    };
}