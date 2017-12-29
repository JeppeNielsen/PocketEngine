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
#include <fstream>
#include <set>

using namespace Pocket;

void FileWorld::AddGameWorld(Pocket::GameWorld &w) {
    GameWorld* world = &w;
    world->GuidToRoot = [this, world] (const std::string& guid) -> GameObject* {
        auto it = guidToPath.find(guid);
        if (it==guidToPath.end()) return 0;
        std::ifstream file;
        file.open(it->second);
        if (!file.is_open()) return 0;
        return nullptr; //TODO: FIX!!! //world->CreateRootFromJson(file, OnRootCreated, OnChildCreated);
    };
    world->GuidToPath = [this, world] (const std::string& guid) {
        auto it = guidToPath.find(guid);
        return it == guidToPath.end() ? "" : it->second;
    };
    world->GetPaths = [this, world] (std::vector<std::string>& guids, std::vector<std::string>& paths) {
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
        std::string guid = GameWorld::ReadGuidFromJson(file);
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
