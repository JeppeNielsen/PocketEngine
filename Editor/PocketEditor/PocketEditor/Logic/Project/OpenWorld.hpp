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

using namespace Pocket;

class Project;
class EditorContext;

class OpenWorld {
public:

    OpenWorld();
    
    std::string Path;
    std::string Filename;
    
    SelectableCollection<EditorObject>* selectables;
    
    bool Save();
    bool Load(const std::string& path, const std::string& filename, EditorContext* context);
    
    static void CreateDefaultSystems(GameObject& root);
    static void CreateEditorSystems(GameObject& root);
    
    void Play();
    void Stop();
    
    Property<bool> IsPlaying;
    
    void Close();
    
    GameObject* Root();
    GameObject* EditorRoot();
    
    void Enable();
    void Disable();
    
    void PreCompile();
    void PostCompile();
    
    Event<> Compiled;
    
    void BindToRoot(GameObject* root);
    
    void Update(InputDevice& input, float dt);
    void Render();
    
private:
    EditorContext* context;
    
    GameObject* root;
    GameObject* editorRoot;
    GameObject* editorCamera;
    
    void InitializeRoot();
    void UpdatePlayMode();
    
    void AddEditorObject(GameObject* object);
    
    GameObject* AddObjectToEditor(GameObject* rootObject);
    
    RunningWorld* runningWorld;
    
    std::stringstream storedWorld;
    std::vector<int> storedSelectedObjects;
    std::map<GameObject*, GameObject*> rootToEditorMap;
    std::vector<ComponentId> editorObjectsComponents;
};

