//
//  TransformAnimation.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/8/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameComponent.hpp"
#include "BezierCurve3.hpp"
#include "TransformAnimation.hpp"
#include <map>

namespace Pocket {
    Component(TransformAnimationDatabase)
    public:
    
        void Reset();
    
        typedef std::map<std::string, TransformAnimation> Animations;
    
        TransformAnimation& AddAnimation(std::string name);
        bool RemoveAnimation(std::string name);
    
        const Animations& GetAnimations();
    
        TransformAnimation* GetAnimation(std::string name);
    
private:
        Animations animations;
    
    
    
    };
}