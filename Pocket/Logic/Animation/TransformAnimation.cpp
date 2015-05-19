//
//  TransformAnimation.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/8/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "TransformAnimation.hpp"
#include "MathHelper.hpp"
#include <cmath>

using namespace Pocket;

TransformAnimation::TransformAnimation() {
    Clear();
}

TransformAnimation::~TransformAnimation() { }

void TransformAnimation::Clear() {
    position.nodes.Clear();
    rotation.nodes.Clear();
    scale.nodes.Clear();
    positionPlayMode = rotationPlayMode = scalePlayMode = TransformAnimationPlayMode::Clamp;
}


float TransformAnimation::GetProgress(float progress, float duration, TransformAnimationPlayMode playMode) {
    switch (playMode) {
        case TransformAnimationPlayMode::Clamp: {
            return progress>duration ? duration : progress;
        }
        case TransformAnimationPlayMode::Wrap: {
            if (progress<0) progress=0;
            return fmodf(progress, duration);
        }
        case TransformAnimationPlayMode::PingPong: {
            return MathHelper::PingPong(progress, 0, duration);
        }
        default:
            return progress;
    }
}

void TransformAnimation::GetValues(Pocket::Vector3 *values, float *progress) {
    values[0] = position.nodes.Count() == 0 ? Vector3(0,0,0) : position.Evaluate(GetProgress(progress[0], position.nodes.GetMaxPosition(), positionPlayMode));
    values[1] = rotation.nodes.Count() == 0 ? Vector3(0,0,0) : rotation.Evaluate(GetProgress(progress[1], rotation.nodes.GetMaxPosition(), rotationPlayMode));
    values[2] = scale.nodes.Count() == 0 ? Vector3(1,1,1) : scale.Evaluate(GetProgress(progress[2], scale.nodes.GetMaxPosition(), scalePlayMode));
}

/*
void TransformAnimation::GetWorld(Pocket::Matrix4x4 &matrix, float *progress) {
    Vector3 pos =
    Vector3 rot = rotation.nodes.Count() == 0 ? Vector3(0,0,0) : rotation.Evaluate(GetProgress(progress[1], rotation.nodes.GetMaxPosition()));
    Vector3 sca = scale.nodes.Count() == 0 ? Vector3(1,1,1) : scale.Evaluate(GetProgress(progress[2], scale.nodes.GetMaxPosition()));
    
    matrix = Matrix4x4::CreateTranslation(pos) * Matrix4x4::CreateRotation(rot.y, rot.x, rot.z) * Matrix4x4::CreateScale(sca);
}
*/

void TransformAnimation::GetPercentageProgresses(float *progress, float percentage) {
    progress[0] = position.nodes.Count() == 0 ? 0 : position.nodes.GetMaxPosition() * percentage;
    progress[1] = rotation.nodes.Count() == 0 ? 0 : rotation.nodes.GetMaxPosition() * percentage;
    progress[2] = scale.nodes.Count() == 0 ? 0 : scale.nodes.GetMaxPosition() * percentage;
}