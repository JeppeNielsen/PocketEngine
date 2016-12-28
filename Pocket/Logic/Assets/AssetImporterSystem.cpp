//
//  AssetImporterSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 27/12/2016.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "AssetImporterSystem.hpp"
#include <iostream>
#include "FileReader.hpp"
#include <fstream>

using namespace Pocket;

const std::string extension = ".json";

AssetImporterSystem::AssetImporterSystem() : isDirty(false) {
    watcher.Changed.Bind([this] () {
        isDirty = true;
    });
}

void AssetImporterSystem::Update(float dt) {
    if (!isDirty) return;
    isDirty = false;
    
    currentFiles.clear();
    FileReader::FindFiles(currentFiles, pathToWatch, "");
    
    for(auto& currentFile : currentFiles) {
        auto prev = std::find(prevFiles.begin(), prevFiles.end(), currentFile);
        if (prev == prevFiles.end()) {
            FileCreated(currentFile);
        }
    }
    
    for(auto& prevFile : prevFiles) {
        auto current = std::find(currentFiles.begin(), currentFiles.end(), prevFile);
        if (current == currentFiles.end()) {
            FileRemoved(prevFile);
        }
    }
    
    prevFiles = currentFiles;
}

void AssetImporterSystem::FileCreated(const std::string &path) {
    std::cout << "AssetImporterSystem::FileCreated: "<<path<<std::endl;
    
    std::string lowerCasePath  = path;
    std::transform(lowerCasePath.begin(), lowerCasePath.end(), lowerCasePath.begin(), ::tolower);
    
    for(GameObject* object : Objects()) {
        AssetImporter* assetImporter = object->GetComponent<AssetImporter>();
        if (FileReader::EndsWith(lowerCasePath, assetImporter->extension)) {
            AssetCreated(assetImporter, path);
        }
    }
}

void AssetImporterSystem::FileRemoved(const std::string &path) {
    std::cout << "AssetImporterSystem::FileRemoved: "<<path<<std::endl;
    
    std::string lowerCasePath  = path;
    std::transform(lowerCasePath.begin(), lowerCasePath.end(), lowerCasePath.begin(), ::tolower);
    
    std::string metaPath = GetMetaPathFromPath(path);
    
    for(GameObject* object : Objects()) {
        AssetImporter* assetImporter = object->GetComponent<AssetImporter>();
        if (FileReader::EndsWith(lowerCasePath, assetImporter->extension) &&
            FileReader::FileExists(metaPath)) {
            FileReader::DeleteFile(metaPath);
        }
    }
}

void AssetImporterSystem::AssetCreated(Pocket::AssetImporter *assetImporter, const std::string &path) {
    std::string metaPath = GetMetaPathFromPath(path);
    
    bool doesMetaExists = FileReader::FileExists(metaPath);
    
    GameWorld world;
    
    if (doesMetaExists) {
        
    } else {
        GameObject* root =world.CreateRoot();
        assetImporter->OnCreated(root);
        
        std::ofstream file;
        file.open(metaPath);
        root->ToJson(file);
        file.close();
    }
}

std::string AssetImporterSystem::GetMetaPathFromPath(const std::string &path) {
    return path + ".meta";
}

void AssetImporterSystem::SetPath(const std::string &path) {
    watcher.Start(path);
    pathToWatch = path;
    isDirty = true;
}
