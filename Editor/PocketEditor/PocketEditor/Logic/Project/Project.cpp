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
#include "TransformHierarchy.hpp"
#include "FileHelper.hpp"
#include <fstream>
#include "FileArchive.hpp"
#include "SystemHelper.hpp"

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
            "/Projects/PocketEngine/Pocket/Logic/Rendering/Renderable.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Gui/Sizeable.hpp",
            "/Projects/PocketEngine/Pocket/Rendering/VertexMesh.hpp",
            "/Projects/PocketEngine/Pocket/Rendering/TextureAtlas.hpp",
            "/Projects/PocketEngine/Pocket/Rendering/Colour.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Interaction/Touchable.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Input/InputController.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Movement/Velocity.hpp",
            "/Projects/PocketEngine/Pocket/Math/MathHelper.hpp",
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

void Project::Initialize(GameWorld &world, FileWorld& fileWorld) {
    this->world = &world;
    this->fileWorld = &fileWorld;
}

ScriptWorld& Project::ScriptWorld() { return scriptWorld; }

void Project::Open(const std::string& path) {
    this->path = path;
    Worlds.Clear();
    
    RefreshSourceFiles();
    
    scriptWorld.SetWorldType(*world, [] (int componentType) {
        return !SystemHelper::IsComponentEditorSpecific(componentType);
    });
    
    scriptWorld.LoadLib();
    scriptWorld.AddGameWorld(*world);
    fileSystemWatcher.Start(path);
    Opened();
}

bool Project::Compile() {
    if (IsCompiling) return false;
    IsCompiling = true;
    
    
    std::cout << "Compilation started..."<< std::endl;
    compilationTimer.Begin();
    
    /*scriptWorld.SetWorldType(*world, [] (int componentType) {
        return !SystemHelper::IsComponentEditorSpecific(componentType);
    });*/
    
    worker.DoTask([this] () -> std::vector<ScriptWorld::Error> {
        RefreshSourceFiles();
        std::vector<ScriptWorld::Error> errors;
        scriptWorld.Build(true, "/Projects/PocketEngine/Projects/PocketEngine/Build/Build/Products/Debug/libPocketEngine.a", [&errors] (const auto& error) {
            errors.push_back(error);
        });
        return errors;
    }, [this] (std::vector<ScriptWorld::Error> errors) {
        if (errors.empty()) {
            Worlds.PreCompile();
            std::stringstream savedComponents;
            world->SerializeAndRemoveComponents(savedComponents, [this] (const GameObject* object, int componentId) {
                return componentId>=scriptWorld.GetBaseComponentIndex();
            });
            
            scriptWorld.RemoveGameWorld(*world);
            scriptWorld.UnloadLib();
            scriptWorld.LoadLib();
            scriptWorld.AddGameWorld(*world);
            world->UpdateActions();
            
            world->DeserializeAndAddComponents(savedComponents);
            world->UpdateActions();
            Worlds.PostCompile();
        }
        compilationTimer.End();
        double time = compilationTimer.End();
        std::cout << "Compilation finished, time = " << time << "s"<< std::endl;
        IsCompiling = false;
        CompilationFinished();
    });
    
    return true;
}

void Project::RefreshSourceFiles() {
    std::vector<std::string> foundIncludeFiles = defaultIncludes;
    FileHelper::RecurseFolder(path, [&] (const std::string& path) {
        foundIncludeFiles.push_back(path);
    }, ".hpp");
    
    std::vector<std::string> foundSourceFiles;
    FileHelper::RecurseFolder(path, [&] (const std::string& path) {
        foundSourceFiles.push_back(path);
    }, ".cpp");
    
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
    
    projectBuilder.Initialize("/Projects/PocketEngine/Pocket", "/Projects/PocketEngine/Editor/TestXCodeProject/WorkingDirectory", path, scriptWorld);
    projectBuilder.SetSourceFiles(foundSourceFiles, foundIncludeFiles);
}

void Project::BuildExecutable(Platform platform, const std::string& outputPath, const std::function<void(const std::string&)>& onOutput) {

    ProjectSettings* projectSettings = GetProjectSettings();
    if (!projectSettings) {
        std::cout << "Failed to find Project Settings"<<std::endl;
        return;
    }
    
    projectBuilder.SetStartupScene(projectSettings->startupScene.SceneGuid());
    projectBuilder.SetProjectName(projectSettings->name);
    projectBuilder.SetBundleIdentifier(projectSettings->bundleIdentifier);
    projectBuilder.SetIconPath(fileWorld->GetPathFromGuid(projectSettings->icon.SceneGuid()));
    
    switch (platform) {
        case Platform::osx: {
            projectBuilder.osx.Build(outputPath, "/Projects/PocketEngine/Projects/PocketEngine/Build/Build/Products/Debug/libPocketEngine.a");
            break;
        }
        case Platform::ios: {
            projectBuilder.ios.Build(outputPath, "/Projects/PocketEngine/Projects/Libraries/iOS/PocketEngine/Build/Build/Products/Debug-iphoneos/libPocketEngine.a");
            break;
        }
        default:
            break;
    }
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
    auto world = Worlds.ActiveWorld();
    return world->EditorRoot() ? world->EditorRoot()->CreateSystem<SelectableCollection<EditorObject>>() : 0;
}

void Project::SaveWorld() {
    /*
    std::cout << " ------------ scenes --------------" << std::endl;
    world->Scenes().Iterate([this] (GameScene* s) {
        std::cout << s->guid << "  -  " << s->root->RootId()<< " -  " << world->GuidToPath(s->guid)  << std::endl;
    });
    std::cout << " ----------------------------------" << std::endl;
	*/

    OpenWorld* activeWorld = Worlds.ActiveWorld;
    if (!activeWorld) return;
    activeWorld->Save();
    world->InvokeChangeToHandles(activeWorld->Root());
}

std::string& Project::Path() { return path; }

FileSystemWatcher* Project::FileSystemWatcher() { return &fileSystemWatcher; }

void Project::CreateSettings(const std::string &path, const std::string& name) {
    GameWorld world;
    
    GameObject* root = world.CreateRoot();
    root->AddComponent<ProjectSettings>()->name = name;
    
    std::ofstream file;
    file.open(path);
    root->ToJson(file);
    file.close();
}

std::string Project::GetFolderName() {
    return FileHelper::GetFileNameFromPath(path);
}

ProjectSettings* Project::GetProjectSettings() {
    std::vector<std::string> guids;
    std::vector<std::string> paths;
    world->GetPaths(guids, paths);
    for (int i=0; i<guids.size(); ++i) {
    
        std::ifstream file;
        file.open(paths[i]);
        int foundId = -1;
        world->TryParseJson(file, GameIdHelper::GetComponentID<ProjectSettings>(), [&, this](int parentId, int id) {
            if (foundId != -1) return;
            foundId = id;
        }, [&, this](const std::string& componentName) -> bool {
            return foundId == -1;
        });
       
        std::cout << "guid: " << guids[i] << "  path:" << paths[i] << std::endl;
        
        if (foundId!=-1) {
            GameObject* object = world->TryFindRoot(guids[i])->FindObject(foundId);
            return object->GetComponent<ProjectSettings>();
        }
    }

    return 0;
}

void Project::Update() {
    worker.Update();
}

ProjectBuilder& Project::Builder() {
    return projectBuilder;
}
