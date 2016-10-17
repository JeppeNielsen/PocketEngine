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
            "/Projects/PocketEngine/Pocket/Logic/Input/InputController.hpp"
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
    
    Worlds.worldDatabase = &worldDatabase;
}

ScriptWorld& Project::ScriptWorld() { return scriptWorld; }

void Project::Open(const std::string& path) {
    this->path = path;
    Worlds.Clear();
    
    GameWorld world;
    OpenWorld::CreateDefaultSystems(world);
    scriptWorld.SetWorldType(world);
    //RefreshSourceFiles();
    //scriptWorld.LoadLib();
    
    RefreshWorldDatabase();
    
    Opened();
}

GameWorld& Project::World() { return Worlds.ActiveWorld()->World(); }

bool Project::Compile() {
    RefreshSourceFiles();
    scriptWorld.Build(true, "/Projects/PocketEngine/Projects/PocketEngine/Build/Build/Products/Debug/libPocketEngine.a");
    return true;
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
    world.AssignUniqueGuid();
    
    GameObject* camera = world.CreateObject();
    camera->AddComponent<Transform>();
    camera->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
    camera->AddComponent<Material>();
    
    std::ofstream file;
    file.open(worldPath);
    world.ToJson(file);
    file.close();
}

void Project::Update(float dt) {
    if (!Worlds.ActiveWorld()) return;
    Worlds.ActiveWorld()->Update(dt);
}

void Project::Render() {
    if (!Worlds.ActiveWorld()) return;
    Worlds.ActiveWorld()->World().Render();
    Worlds.ActiveWorld()->EditorWorld().Render();
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
GameWorldDatabase& Project::WorldDatabase() { return worldDatabase; }

void Project::RefreshWorldDatabase() {
    std::vector<std::string> files;
    FileHelper::FindFiles(files, path, ".json");
    worldDatabase.Clear();
    for(auto& f : files) {
        worldDatabase.AddPath(f);
    }
    
    std::cout << "Guid paths:"<<std::endl;
    for(auto d : worldDatabase.GetPaths()) {
        std::cout << "GUID: " << d.first << " : " << d.second<<std::endl;
    }
}
