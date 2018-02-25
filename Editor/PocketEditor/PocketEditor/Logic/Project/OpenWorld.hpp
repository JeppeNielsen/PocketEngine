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
#include "FileWorld.hpp"
#include <sstream>
#include "RunningWorld.hpp"
#include "EditorScene.hpp"
#include "GameObjectJsonSerializer.hpp"

using namespace Pocket;

class Project;
class EditorContext;

class OpenWorld {
public:

    OpenWorld();
    
    std::string Path;
    std::string Filename;
    
    bool Save();
    bool Load(const std::string& path, const std::string& filename, EditorContext* context);
    
    void Play();
    void Stop();
    
    Property<bool> IsPlaying;
    Property<bool> IsPaused;
    
    void Close();
    
    GameObject* Root();
    
    void Enable();
    void Disable();
    
    void PreCompile();
    void PostCompile();
    
    void Update(InputDevice& input, float dt);
    void Render();

    Property<GameObject*> GameRoot;
    Property<GameObject*> EditorRoot;
    
    RunningWorld* GetRunningWorld();
    
    GameWorld* World();
    
private:
    GameObjectJsonSerializer serializer;
    EditorContext* context;
    
    GameWorld world;
    GameObject* prefab;
    GameObject* scene;
    RunningWorld* runningWorld;
    
    EditorScene editorScene;
};

