//
//  AssetImporterSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 27/12/2016.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "AssetImporterSystem.hpp"
#include <iostream>
#include "FileHelper.hpp"
#include <fstream>
#include "StringHelper.hpp"
#include "GameObjectJsonSerializer.hpp"

using namespace Pocket;

const std::string extension = ".json";

AssetImporterSystem::AssetImporterSystem() : isDirty(false), watcher(0) {}

void AssetImporterSystem::Initialize() {
}

void AssetImporterSystem::Destroy() {
    if (watcher) {
        this->watcher->Changed.Unbind(this, &AssetImporterSystem::FilesChanged);
    }
}

void AssetImporterSystem::SetFileWatcher(Pocket::FileSystemWatcher *watcher) {
    this->watcher = watcher;
    this->watcher->Changed.Bind(this, &AssetImporterSystem::FilesChanged);
    isDirty = true;
}

void AssetImporterSystem::Update(float dt) {
    if (!isDirty) return;
    isDirty = false;
    
    currentFiles.clear();
    FileHelper::RecurseFolder(watcher->Path(), [&] (const std::string& p) {
        currentFiles.push_back(p);
    });
    
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

void AssetImporterSystem::FilesChanged() {
    isDirty = true;
}

void AssetImporterSystem::FileCreated(const std::string &path) {
    std::cout << "AssetImporterSystem::FileCreated: "<<path<<std::endl;
    
    std::string lowerCasePath  = path;
    std::transform(lowerCasePath.begin(), lowerCasePath.end(), lowerCasePath.begin(), ::tolower);
    
    for(GameObject* object : Objects()) {
        AssetImporter* assetImporter = object->GetComponent<AssetImporter>();
        if (StringHelper::EndsWith(lowerCasePath, assetImporter->extension)) {
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
        if (StringHelper::EndsWith(lowerCasePath, assetImporter->extension) &&
            FileHelper::FileExists(metaPath)) {
            FileHelper::DeleteFile(metaPath);
        }
    }
}

void AssetImporterSystem::AssetCreated(Pocket::AssetImporter *assetImporter, const std::string &path) {
    std::string metaPath = GetMetaPathFromPath(path);
    
    bool doesMetaExists = FileHelper::FileExists(metaPath);
    
    if (!doesMetaExists) {
        GameWorld world(this->root->World()->Storage());
    
        GameObject* root = world.CreateScene();
        assetImporter->OnCreated(root);
        
        GameObjectJsonSerializer serializer;
        
        std::ofstream file;
        file.open(metaPath);
        serializer.Serialize(root, file);
        file.close();
    }
}

std::string AssetImporterSystem::GetMetaPathFromPath(const std::string &path) {
    return path + ".meta";
}
