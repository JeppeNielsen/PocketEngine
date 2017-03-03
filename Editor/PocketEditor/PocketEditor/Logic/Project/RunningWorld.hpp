//
//  RunningWorld.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 01/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "FileWorld.hpp"

using namespace Pocket;

class RunningWorld {
public:
    void Initialize(const std::string& path, const std::vector<std::string>& startScenes, ScriptWorld& scriptWorld);
    
    GameWorld& World();

private:
    GameWorld world;
    FileWorld fileWorld;
};
