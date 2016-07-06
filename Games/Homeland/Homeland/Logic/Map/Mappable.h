//
//  Mappable.h
//  Homeland
//
//  Created by Jeppe Nielsen on 23/07/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Map.h"

using namespace Pocket;

struct Mappable {
    Mappable();
    Map* Map;
};
