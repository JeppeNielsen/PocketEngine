//
//  TransformAnimation.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/8/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "TransformAnimationDatabase.hpp"

using namespace Pocket;

TransformAnimationDatabase::TransformAnimationDatabase() {
    animations.clear();
}

TransformAnimation& TransformAnimationDatabase::AddAnimation(std::string name) {
    return animations[name];
}

bool TransformAnimationDatabase::RemoveAnimation(std::string name) {
    Animations::iterator it = animations.find(name);
    if (it!=animations.end()) {
        animations.erase(it);
        return true;
    }
    return false;
}

const TransformAnimationDatabase::Animations& TransformAnimationDatabase::GetAnimations() {
    return animations;
}

TransformAnimation* TransformAnimationDatabase::GetAnimation(std::string name) {
    Animations::iterator it = animations.find(name);
    if (it!=animations.end()) {
        return &it->second;
    } else {
        return 0;
    }
}