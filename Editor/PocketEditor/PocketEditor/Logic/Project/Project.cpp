//
//  Project.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Project.hpp"
#include "RenderSystem.hpp"
#include "TouchSystem.hpp"
#include "EditorObjectCreatorSystem.hpp"
#include "TransformHierarchy.hpp"
#include "FileHelper.hpp"
#include <fstream>

Project::Project() {
    scriptWorld.Types.Add<Vector3>();

    scriptWorld.SetClangSdkPath("/Users/Jeppe/Downloads/clang+llvm-3.7.0-x86_64-apple-darwin/");
    
    defaultIncludes =
    {
            "/Projects/PocketEngine/Pocket/Data/Property.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Spatial/Transform.hpp",
            "/Projects/PocketEngine/Pocket/Math/Vector2.hpp",
            "/Projects/PocketEngine/Pocket/Math/Vector3.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Rendering/Mesh.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Gui/Sizeable.hpp",
            "/Projects/PocketEngine/Pocket/Rendering/VertexMesh.hpp",
            "/Projects/PocketEngine/Pocket/Rendering/TextureAtlas.hpp",
            "/Projects/PocketEngine/Pocket/Rendering/Colour.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Interaction/Touchable.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Input/InputController.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Movement/Velocity.hpp",
        };
    
    /*
    scriptWorld.SetFiles(
        "ScriptExample.so",
        "/Projects/PocketEngine/Editor/PocketEditor/PocketEditor/ScriptInclude",
        {
            "/Projects/PocketEngine/Editor/PocketEditor/PocketEditor/ScriptCode/ScriptExample.hpp",
        },
        defaultIncludes
    );
    */
    
    Worlds.ActiveWorld.Changed.Bind([this] () {
        OpenWorld* current = Worlds.ActiveWorld();
        OpenWorld* old = Worlds.ActiveWorld.PreviousValue();
        if (current) {
            current->Enable();
        }
        
        if (old) {
            old->Disable();
        }
    });
}

void Project::Initialize(Pocket::GameWorld &world) {
    this->world = &world;
}

ScriptWorld& Project::ScriptWorld() { return scriptWorld; }

void Project::Open(const std::string& path) {
    this->path = path;
    Worlds.Clear();
    
    //RefreshSourceFiles();
    //scriptWorld.LoadLib();
    Opened();
}

bool Project::Compile() {
    PostCompile();
    Worlds.PreCompile();
    scriptWorld.RemoveGameWorld(*world);
    RefreshSourceFiles();
    scriptWorld.SetWorldType(*world);
    bool succes = scriptWorld.Build(true, "/Projects/PocketEngine/Projects/PocketEngine/Build/Build/Products/Debug/libPocketEngine.a");
    if (succes) {
        scriptWorld.AddGameWorld(*world);
        Worlds.PostCompile();
    }
    return succes;
}

void Project::RefreshSourceFiles() {
    std::vector<std::string> foundIncludeFiles = defaultIncludes;
    FileHelper::FindFiles(foundIncludeFiles, path, ".hpp");
    
    std::vector<std::string> foundSourceFiles;
    FileHelper::FindFiles(foundSourceFiles, path, ".cpp");
    
    for(auto s : foundIncludeFiles) {
        std::cout << "Includes : "<< s <<std::endl;
    }
     for(auto s : foundSourceFiles) {
        std::cout << "Source : "<< s <<std::endl;
    }
    
    scriptWorld.SetFiles(
        "ScriptExample.so",
        "/Projects/PocketEngine/Editor/PocketEditor/PocketEditor/ScriptInclude",
        foundSourceFiles,
        foundIncludeFiles
    );
    
    scriptWorld.ExtractScriptClasses();
}

void Project::Build() {
    /*
    scriptWorld.BuildExecutable("/Projects/PocketEngine/Projects/PocketEngine/Build/Build/Products/Debug/libPocketEngine.a");
    std::ofstream file;
    file.open ("world.json");
    world.ToJson(file);
    file.close();
    */
}

void Project::CreateNewWorld(const std::string &worldPath) {
    GameWorld world;
    
    GameObject* root = world.CreateRoot();
    
    GameObject* cube = root->CreateObject();
    cube->AddComponent<Transform>();
    cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
    cube->AddComponent<Renderable>();
    
    std::ofstream file;
    file.open(worldPath);
    root->ToJson(file);
    file.close();
}

SelectableCollection<EditorObject>* Project::GetSelectables() {
    if (!Worlds.ActiveWorld()) return 0;
    return Worlds.ActiveWorld()->selectables;
}

void Project::SaveWorld() {
    if (!Worlds.ActiveWorld) return;
    Worlds.ActiveWorld()->Save();
}

std::string& Project::Path() { return path; }
