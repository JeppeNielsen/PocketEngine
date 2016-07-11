//
//  Creator.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 02/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "GameWorld.hpp"

using namespace Pocket;

struct Creator {
    Creator();
    GameObject* particle;
    GameObject* spring;
};