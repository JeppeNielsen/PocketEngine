//
//  FileWorld.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 13/11/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "FileWorld.hpp"
#include "GameWorld.hpp"
#include "FileHelper.hpp"
#include "GameStorage.hpp"
#include <fstream>
#include <set>
#include "GameObjectJsonSerializer.hpp"

using namespace Pocket;

void FileWorld::Initialize(Pocket::GameStorage& storage) {
    storage.GuidToRoot = [this, &storage] (const std::string& guid) -> GameObject* {
        auto it = guidToPath.find(guid);
        if (it==guidToPath.end()) return nullptr;
        std::ifstream file;
        file.open(it->second);
        if (!file.is_open()) return nullptr;
        return storage.LoadPrefab(guid, file);
    };
    storage.GuidToPath = [this] (const std::string& guid) {
        auto it = guidToPath.find(guid);
        return it == guidToPath.end() ? "" : it->second;
    };
    storage.GetPaths = [this] (std::vector<std::string>& guids, std::vector<std::string>& paths) {
        for(auto p : guidToPath) {
            guids.push_back(p.first);
            paths.push_back(p.second);
        }
    };
}

void FileWorld::FindRoots(const std::string &path, const std::vector<std::string>& extensions) {
    std::set<std::string> uniquePaths;
    for(auto& ext : extensions) {
        FileHelper::RecurseFolder(path, [&] (const std::string& path) {
            uniquePaths.insert(path);
        }, ext);
    }
    guidToPath.clear();
    for(auto& path : uniquePaths) {
        std::ifstream file;
        file.open(path);
        if (!file.is_open()) continue;
        std::string guid = GameObjectJsonSerializer::ReadGuidFromJson(file);
        if (guid!="") {
            guidToPath[guid] = path;
        }
    }
}

const std::map<std::string, std::string>& FileWorld::Paths() {
    return guidToPath;
}

std::string FileWorld::GetPathFromGuid(const std::string &guid) {
    auto it = guidToPath.find(guid);
    return it == guidToPath.end() ? "" : it->second;
}
