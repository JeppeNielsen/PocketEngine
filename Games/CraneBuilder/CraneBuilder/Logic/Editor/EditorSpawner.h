//
//  EditorSpawner.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 09/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include <map>
#include "Vector3.hpp"

using namespace Pocket;

struct EditorSpawner {
    EditorSpawner();
    typedef std::map<std::string, std::function<GameObject*(Vector3 p)>> StateObjects;
    typedef std::map<std::string, StateObjects> Objects;
    Objects objects;
    std::string CurrentObject;
};