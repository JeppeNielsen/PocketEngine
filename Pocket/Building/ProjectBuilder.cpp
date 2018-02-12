//
//  ProjectBuilder.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 24/06/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "ProjectBuilder.hpp"
#include "FileHelper.hpp"
#include <iostream>
#include <fstream>
#include "StringHelper.hpp"
#include "FileReader.hpp"
#include "FileArchive.hpp"
#include "GameWorld.hpp"
#include "GameObjectJsonSerializer.hpp"

using namespace Pocket;

ProjectBuilder::ProjectBuilder() {
    ios.project = this;
    osx.project = this;
}

void ProjectBuilder::Initialize(const std::string &pocketEngineIncludePath, const std::string &workingDirectory, const std::string& resourcesPath, ScriptWorld& world) {
    this->pocketEngineIncludePath = pocketEngineIncludePath;
    this->workingDirectory = workingDirectory;
    this->resourcesPath = resourcesPath;
    this->world = &world;
}

void ProjectBuilder::SetSourceFiles(const std::vector<std::string> &sourceFiles, const std::vector<std::string> &headerFiles) {
    this->sourceFiles = sourceFiles;
    this->headerFiles = headerFiles;
}

std::vector<std::string> ProjectBuilder::GetEngineHeaders() {
    return {
    "ComponentSystem",
    "ComponentSystem/Meta",
    "OpenGL",
    "Math",
    "Data",
    "Core",
    "Platform/OSX",
    "Serialization",
    
    "Logic/Animation",
    "Logic/Audio",
    "Logic/Common",
    "Logic/Editor",
    "Logic/Effects",
    "Logic/Gui",
    "Logic/Gui/Layout",
    "Logic/Gui/Menu",
    
    "Logic/Interaction",
    
    "Logic/Movement",
    "Logic/Physics",
    "Logic/Rendering",
    "Logic/Selection",
    "Logic/Spatial",
    "Logic/Cloning",
    "Logic/Switching",
    "Logic/Assets",
    "Logic/Input",
    "Logic/Switching",
    "Logic/Triggering",
    "Logic/Scenes",
    "Logic/Spawning",
    
    "Rendering/",
    "Libs/Zip",
    "Libs/STBImage",
    "Libs/FreeType/Include",
    "Libs/TinyXml",
    };
}

bool ProjectBuilder::CreateResources(const std::string &outputFile) {
    bool wasCreated = FileArchive::TryCreateArchiveFile(resourcesPath, outputFile, [] (const std::string& path) -> std::string {
        std::string metaPath = path + ".meta";
        
        if (FileHelper::FileExists(metaPath)) {
            std::ifstream file;
            file.open(metaPath);
            std::string guid = GameObjectJsonSerializer::ReadGuidFromJson(file);
            if (guid == "") return "";
            return guid + "-asset";
        } else {
            
            std::ifstream file;
            file.open(path);
            return GameObjectJsonSerializer::ReadGuidFromJson(file);
        }
    });

    return wasCreated;
}

void ProjectBuilder::SetStartupScene(const std::string &sceneGUID) {
    startupSceneGUID = sceneGUID;
}

void ProjectBuilder::SetProjectName(const std::string &projectName) {
    this->projectName = projectName;
}

void ProjectBuilder::SetBundleIdentifier(const std::string &bundleIdentifier) {
    this->bundleIdentifier = bundleIdentifier;
}

void ProjectBuilder::SetIconPath(const std::string &iconPath) {
    this->iconPath = iconPath;
}

