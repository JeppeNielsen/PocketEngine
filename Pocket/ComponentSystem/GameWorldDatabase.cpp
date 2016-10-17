//
//  GameWorldDatabase.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 16/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameWorldDatabase.hpp"
#include "minijson_reader.hpp"
#include <sys/stat.h>

using namespace Pocket;

std::string GameWorldDatabase::ReadGuidFromStream(std::istream &jsonStream) {
    std::string guid;
    minijson::istream_context context(jsonStream);
    minijson::parse_object(context, [&] (const char* n, minijson::value v) {
        std::string name = n;
        if (name == "World" && v.type() == minijson::Object) {
            minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                std::string id = n;
                if (id == "id" && v.type() == minijson::String) {
                    guid = v.as_string();
                } else {
                    minijson::ignore(context);
                    
                }
            });
        } else {
            minijson::ignore(context);
        }
    });
    return guid;
}

void GameWorldDatabase::AddPath(const std::string &path) {

    std::ifstream file;
    file.open(path);
    std::string guid = ReadGuidFromStream(file);
    file.close();
    
    if (guid!="") {
        paths[guid] = path;
    }
}

std::string GameWorldDatabase::GetPath(const std::string &guid) {
    return paths[guid];
}

bool GameWorldDatabase::DoesWorldExists(const std::string &guid) {
    auto it = paths.find(guid);
    if (it==paths.end()) return false;
    struct stat buffer;
    return (stat (it->second.c_str(), &buffer) == 0);
}

void GameWorldDatabase::RemoveInvalidPaths() {
    for(auto it = paths.begin(); it!=paths.end(); ++it) {
        if (!DoesWorldExists(it->first)) {
            it = paths.erase(it);
        }
    }
}

void GameWorldDatabase::Clear() {
    paths.clear();
}

const GameWorldDatabase::Paths& GameWorldDatabase::GetPaths() { return paths; }

bool GameWorldDatabase::TryGetWorld(const std::string &guid, Pocket::GameWorld **worldOut) {
    GameWorld* world = 0;
    auto it = loadedWorlds.find(guid);
    if (it == loadedWorlds.end()) {
        if (!DoesWorldExists(guid)) {
            *worldOut = 0;
            return false;
        }
        world = new GameWorld();
        world->SetDatabase(this);
        
        std::ifstream file;
        file.open(paths[guid]);
        world->CreateObject(file);
        file.close();
        
        loadedWorlds[guid] = world;
    } else {
        world = it->second;
    }
    *worldOut = world;
    return true;
}

GameObject* GameWorldDatabase::FindGameObject(const std::string &guidWorld, const std::string& objectID) {
    GameWorld* world;
    if (!TryGetWorld(guidWorld, &world)) {
        return 0;
    }
    return world->Root()->Children()[0];
}

