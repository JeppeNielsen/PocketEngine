//
//  TerrainEditableVertices.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 07/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include <vector>

using namespace Pocket;

struct TerrainEditableVertices {
    typedef std::vector<GameObject*> Vertices;
    Vertices vertices;
};