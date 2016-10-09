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
#include <sstream>

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
    
    void Play();
    void Stop();
    
    Property<bool> IsPlaying;
    
    void Update(float dt);
    
private:
    GameWorld editorWorld;
    GameWorld world;
    std::stringstream storedWorld;
};

