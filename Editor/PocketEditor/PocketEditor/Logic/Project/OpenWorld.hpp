//
//  OpenWorld.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 13/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "InputManager.hpp"
#include "SelectableCollection.hpp"
#include "EditorObject.hpp"

using namespace Pocket;

class OpenWorld {
public:
    GameWorld& World();
    GameWorld& EditorWorld();
    
    std::string Path;
    std::string Filename;
    
    void CreateDefault();
    
    SelectableCollection<EditorObject>* selectables;
    
    bool Save();
    bool Load(const std::string& path, const std::string& filename, ScriptWorld& scriptWorld);
    
    static void CreateDefaultSystems(GameWorld& world);
    
private:
    GameWorld editorWorld;
    GameWorld world;
};

