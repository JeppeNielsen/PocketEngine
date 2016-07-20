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
    scriptWorld.RemoveGameWorld(world);
    world.Clear();
    this->path = path;
}

GameWorld& Project::World() { return world; }

void Project::CreateDefaultScene(GameWorld& editorWorld, GameObject* gameRoot, InputManager& input) {

    world.CreateSystem<RenderSystem>();
    world.CreateSystem<TransformHierarchy>();
    world.CreateSystem<TouchSystem>()->Input = &input;
    world.CreateSystem<TouchSystem>()->TouchDepth = 0;
    auto creatorSystem = world.CreateSystem<EditorObjectCreatorSystem>();
    creatorSystem->editorWorld = &editorWorld;
    creatorSystem->gameRoot = gameRoot;
    
    GameObject* camera = world.CreateObject();
    camera->AddComponent<Camera>();
    camera->AddComponent<Transform>()->Position = { 0, 0, 10 };
    camera->GetComponent<Camera>()->FieldOfView = 70;

    return;
    
    for (int x=-1; x<=1; ++x) {
    for (int y=-1; y<=1; ++y) {
    
        GameObject* cube = world.CreateObject();
        cube->AddComponent<Transform>()->Position = {x*2.3f,y*2.3f,0};
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 0.2f);
        cube->AddComponent<Material>();
        cube->AddComponent<EditorObject>();
        
        auto& verts = cube->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
        
        for (int i=0; i<verts.size(); i++) {
            verts[i].Color = Colour::HslToRgb(i * 10, 1, 1, 1);
        }
        
        GameObject* child = world.CreateObject();
        child->Parent() = cube;
        child->AddComponent<Transform>()->Position = {0,0,-2};
        child->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 0.5f);
        child->AddComponent<Material>();
        child->AddComponent<EditorObject>();
        }
    }
}

bool Project::Compile() {
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
    
    return true;
}

void Project::Build() {
    scriptWorld.BuildExecutable("/Projects/PocketEngine/Projects/PocketEngine/Build/Build/Products/Debug/libPocketEngine.a");
    std::ofstream file;
    file.open ("world.json");
    world.ToJson(file);
    file.close();
}

