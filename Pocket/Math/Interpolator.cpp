//
//  Interpolator.cpp
//  Typhoon
//
//  Created by Jeppe Nielsen on 1/18/12.
//  Copyright (c) 2012 Crowdplayce. All rights reserved.
//

#include "Interpolator.hpp"
#include "TransitionHelper.hpp"

using namespace Pocket;

Interpolator::Interpolator() {
    Start = 0;
    End = 0;
    StartTangent = 0.5f;
    EndTangent = 0.5f;
    Tension = 0;
    Bias = 0;
}

Interpolator::~Interpolator() {}

Interpolator::Interpolator(float start, float end, float startTangent, float endTangent, float tension, float bias) {
    this->Start = start;
    this->End = end;
    this->StartTangent = startTangent;
    this->EndTangent = endTangent;
    this->Tension = tension;
    this->Bias = bias;
}

float Interpolator::Interpolate(const float& value) {
    return TransitionHelper::HermiteInterpolate(value, Start, End, StartTangent, EndTangent, Tension, Bias);
}

Pocket::Interpolator Interpolator::Ease() {
    return Interpolator(0, 1, 0, 1, 0, 0);
}

Pocket::Interpolator Interpolator::EaseIn() {
    return Interpolator(0, 1, 0, 0, 0, 0);
}

Pocket::Interpolator Interpolator::EaseOut() {
    return Interpolator(0, 1, 1, 1, 0, 0);
}