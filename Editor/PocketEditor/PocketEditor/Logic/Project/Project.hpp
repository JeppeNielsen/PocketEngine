//
//  Project.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Transform.hpp"
#include "InputManager.hpp"
#include "ScriptWorld.hpp"
#include <vector>
#include "OpenWorldCollection.hpp"
#include "SelectableCollection.hpp"
#include "EditorObject.hpp"

using namespace Pocket;

class Project {
private:
    
    ScriptWorld scriptWorld;
    std::string path;
    std::vector<std::string> defaultIncludes;
    
    void RefreshSourceFiles();
    
public:
    OpenWorldCollection Worlds;

    Project();
    void Open(const std::string& path);
    GameWorld& World();
    ScriptWorld& ScriptWorld();
    void CreateDefaultScene(GameWorld& editorWorld, GameObject* gameRoot, InputManager& input);
    bool Compile();
    void Build();
    void CreateNewWorld(const std::string& worldPath);
    
    void Update(float dt);
    void Render();
    
    void SaveWorld();
    
    SelectableCollection<EditorObject>* GetSelectables();
    
    std::string& Path();
    
    Event<> Opened;
};