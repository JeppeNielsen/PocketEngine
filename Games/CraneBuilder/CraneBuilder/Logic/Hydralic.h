//
//  Hydralic.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 02/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
Component(Hydralic)
    void Reset();
    float minLength;
    float maxLength;
    float speed;
};