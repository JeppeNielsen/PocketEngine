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
#include "FileSystemWatcher.hpp"
#include "ProjectSettings.hpp"
#include "Worker.hpp"
#include "Timer.hpp"
#include "ProjectBuilder.hpp"

using namespace Pocket;

class Project {
private:
    
    std::vector<std::string> defaultIncludes;
    GameStorage* storage;
    FileWorld* fileWorld;
    ScriptWorld* scriptWorld;
    
    std::string path;
    FileSystemWatcher fileSystemWatcher;
    Worker worker;
    
    void RefreshSourceFiles();
    
    ProjectBuilder projectBuilder;
    
public:
    OpenWorldCollection Worlds;

    Project();
    
    void Initialize(GameStorage& storage, FileWorld& fileWorld, ScriptWorld& scriptWorld);
    
    void Open(const std::string& path);
    void CreateDefaultScene(GameWorld& editorWorld, GameObject* gameRoot, InputManager& input);
    bool Compile();
    
    enum class Platform { osx, ios };
    
    void BuildExecutable(Platform platform, const std::string& outputPath, const std::function<void(const std::string&)>& onOutput = 0);
    void CreateNewWorld(const std::string& worldPath);
    
    void SaveWorld();
    
    void Update();
    
    SelectableCollection<EditorObject>* GetSelectables();
    
    std::string& Path();
    
    FileSystemWatcher* FileSystemWatcher();
    
    void CreateSettings(const std::string& path, const std::string& name);
    
    ProjectSettings* GetProjectSettings();
    
    std::string GetFolderName();
    
    Event<> Opened;
    Property<bool> IsCompiling;
    Event<> CompilationFinished;
    Timer compilationTimer;
    
    ProjectBuilder& Builder();
};
