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
    
private:
    GameWorld world;
    GameWorld editorWorld;
};
