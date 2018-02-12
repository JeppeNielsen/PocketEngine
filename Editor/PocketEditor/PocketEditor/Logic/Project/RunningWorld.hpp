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
#include "EditorScene.hpp"

using namespace Pocket;

class RunningWorld {
public:

    RunningWorld();

    void Initialize(const std::string& path, const std::vector<std::string>& startScenes, GameStorage& storage);
    void Destroy();
    
    GameWorld& World();
    
    Property<GameObject*> ActiveScene;
    GameObject* EditorRoot();
    
    void PreCompile();
    void PostCompile();

private:
    GameWorld world;
    EditorScene editorScene;
    
    std::stringstream serializedComponents;
};
