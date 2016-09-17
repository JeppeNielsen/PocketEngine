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
            "/Projects/PocketEngine/Pocket/Logic/Interaction/Touchable.hpp"
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
}

ScriptWorld& Project::ScriptWorld() { return scriptWorld; }

void Project::CreateNew(const std::string& path) {
    //scriptWorld.RemoveGameWorld(world);
    //world.Clear();
    this->path = path;
    Worlds.Clear();
}

GameWorld& Project::World() { return Worlds.ActiveWorld()->World(); }


bool Project::Compile() {
    /*
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
    
    scriptWorld.RemoveGameWorld(world);
    scriptWorld.SetWorldType(world);
    scriptWorld.Build(true);
    scriptWorld.AddGameWorld(world);
    */
    return true;
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
    
    GameObject* camera = world.CreateObject();
    camera->AddComponent<Camera>();
    camera->AddComponent<Transform>()->Position = { 0, 0, 10 };
    camera->GetComponent<Camera>()->FieldOfView = 70;
    
    std::ofstream file;
    file.open(path + worldPath);
    world.ToJson(file);
    file.close();
}

void Project::Update(float dt) {
    if (!Worlds.ActiveWorld()) return;
    Worlds.ActiveWorld()->World().Update(dt);
    Worlds.ActiveWorld()->EditorWorld().Update(dt);
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

