//
//  Constructor.h
//  Homeland
//
//  Created by Jeppe Nielsen on 29/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "GameWorld.hpp"

Component(Constructor)
    Pointer<GameObject> ObjectToConstruct;
    Vector3 contructorPosition;
};