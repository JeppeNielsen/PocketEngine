//
//  TransitionHelper.cpp
//  FlingTheFishX
//
//  Created by Jeppe Nielsen on 8/5/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#include "TransitionHelper.hpp"
#include <math.h>

//#define M_PI 3.1459

using namespace Pocket;

float TransitionHelper::easeOutElastic(float start, float end, float value) {

    end -= start;
    
    float d = 1;
    float p = d * .3f;
    float s = 0;
    float a = 0;
    
    if (value == 0) return start;
    
    if ((value /= d) == 1) return start + end;
        
    if (a == 0 || a < (end<0 ? -end : end)){
        a = end;
        s = p / 4;
    }else{
        s = p / (2 * (float)M_PI) * asinf(end / a);
    }
    
    return (a * powf(2, -10 * value) *sinf((value * d - s) * (2 * (float)M_PI) / p) + end + start);
}		


float TransitionHelper::punch(float amplitude, float value){
    float s = 9;
    if (value == 0){
        return 0;
    }
    if (value == 1){
        return 0;
    }
    float period = 1 * 0.3f;
    s = period / (2 * (float)M_PI) * asinf(0);
    return (amplitude * powf(2, -10 * value) * sinf((value * 1 - s) * (2 * (float)M_PI) / period));
}


float TransitionHelper::easeInElastic(float start, float end, float value){
    end -= start;
    
    float d = 1.0f;
    float p = d * .3f;
    float s = 0;
    float a = 0;
    
    if (value == 0) return start;
    
    if ((value /= d) == 1) return start + end;
    
    if (a == 0 || a < (end<0 ? -end : end)){
        a = end;
        s = p / 4;
    }else{
        s = p / (2 * (float)M_PI) * asinf(end / a);
    }
    
    return -(a * powf(2, 10 * (value)) * sinf(((value-1) * d - s) * (2 * (float)M_PI) / p)) + start;
}		


float TransitionHelper::CalcAngle(float Ang1, float Ang2) {
    
    float fDif = Ang2-Ang1;
    if (fDif >= M_PI) {
        fDif -= M_PI * 2;
    }
    else if (fDif <= -M_PI) {
        fDif += M_PI * 2;
    }
    return fDif;
}


float TransitionHelper::HermiteInterpolate(float mu, float y1, float y2, float y0, float y3, float tension, float bias) {
   
    
    float m0, m1, mu2, mu3;
    float a0, a1, a2, a3;
    float tensionHalf = (1 - tension) * 0.5f;
    
    mu2 = mu * mu;
    mu3 = mu2 * mu;
    m0 = (y1 - y0) * (1 + bias) * tensionHalf;
    m0 = m0 + (y2 - y1) * (1 - bias) * tensionHalf;
    m1 = (y2 - y1) * (1 + bias) * tensionHalf;
    m1 = m1 + (y3 - y2) * (1 - bias) * tensionHalf;
    a0 = 2.0f * mu3 - 3 * mu2 + 1.0f;
    a1 = mu3 - 2.0f * mu2 + mu;
    a2 = mu3 - mu2;
    a3 = -2.0f * mu3 + 3.0f * mu2;
    
    return a0 * y1 + a1 * m0 + a2 * m1 + a3 * y2;
}

