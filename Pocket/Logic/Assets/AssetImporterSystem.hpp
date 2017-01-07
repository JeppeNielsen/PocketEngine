//
//  AssetImporterSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 27/12/2016.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "AssetImporter.hpp"
#include "FileSystemWatcher.hpp"
#include "AssetLoader.hpp"
#include <vector>

namespace Pocket {
    class AssetImporterSystem : public GameSystem<AssetImporter> {
    protected:
        void Initialize();
        void Destroy();
        void Update(float dt);
    
    public:
        AssetImporterSystem();
        
        void SetFileWatcher(FileSystemWatcher* watcher);
        
    private:
        void FilesChanged();
        void FileCreated(const std::string& path);
        void FileRemoved(const std::string& path);
        
        void AssetCreated(AssetImporter* assetImporter, const std::string& path);
        std::string GetMetaPathFromPath(const std::string& path);
        
        bool isDirty;
        std::string pathToWatch;
        
        using FileList = std::vector<std::string>;
        
        FileList currentFiles;
        FileList prevFiles;
        
        FileSystemWatcher* watcher;
    };
}
