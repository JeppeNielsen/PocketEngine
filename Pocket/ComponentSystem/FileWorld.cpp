//
//  FileWorld.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 13/11/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "FileWorld.hpp"
#include "GameWorld.hpp"
#include "FileReader.hpp"
#include <fstream>

using namespace Pocket;

void FileWorld::AddGameWorld(Pocket::GameWorld &w) {
    world = &w;
    world->GuidToRoot = [this] (const std::string& guid) -> GameObject* {
        auto it = guidToPath.find(guid);
        if (it==guidToPath.end()) return 0;
        std::ifstream file;
        file.open(it->second);
        if (!file.is_open()) return 0;
        return world->CreateRootFromJson(file, OnRootCreated, OnChildCreated);
    };
}

void FileWorld::FindRoots(const std::string &path, const std::string &extension) {
    std::vector<std::string> paths;
    FileReader::FindFiles(paths, path, extension);
    
    guidToPath.clear();
    for(auto& path : paths) {
        std::ifstream file;
        file.open(path);
        if (!file.is_open()) continue;
        std::string guid = world->ReadGuidFromJson(file);
        if (guid!="") {
            guidToPath[guid] = path;
        }
    }
}


