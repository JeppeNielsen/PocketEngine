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
#include <vector>

namespace Pocket {
    class AssetImporterSystem : public GameSystem<AssetImporter> {
    protected:
        void Update(float dt);
    
    public:
        AssetImporterSystem();
        
        void SetPath(const std::string& path);
        
    private:
    
        void FileCreated(const std::string& path);
        void FileRemoved(const std::string& path);
        
        void AssetCreated(AssetImporter* assetImporter, const std::string& path);
        std::string GetMetaPathFromPath(const std::string& path);
        
        FileSystemWatcher watcher;
        bool isDirty;
        std::string pathToWatch;
        
        using FileList = std::vector<std::string>;
        
        FileList currentFiles;
        FileList prevFiles;
    };
}
