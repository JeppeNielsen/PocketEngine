//
//  OpenWorldCollection.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 13/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "OpenWorldCollection.hpp"
#include <fstream>

OpenWorldCollection::OpenWorldCollection() {
    ActiveWorld = 0;
}

OpenWorldCollection::~OpenWorldCollection() {
    Clear();
}

void OpenWorldCollection::Clear() {
    for(auto w : worlds) {
        delete w;
    }
    worlds.clear();
    ActiveWorld = 0;
}

bool OpenWorldCollection::TryFindOpenWorld(const std::string &path, OpenWorld** world) {
    for(auto w : worlds) {
        if (w->Path == path) {
            *world = w;
            return w;
        }
    }
    *world = 0;
    return false;
}

OpenWorld* OpenWorldCollection::LoadWorld(const std::string &path, const std::string& filename, ScriptWorld& scriptWorld) {
    OpenWorld* world;
    if (!TryFindOpenWorld(path, &world)) {
        world = new OpenWorld();
        world->Load(path, filename, scriptWorld);
        worlds.push_back(world);
        WorldLoaded(world);
    }
    ActiveWorld = world;
    return world;
}

void OpenWorldCollection::CloseWorld(OpenWorld *world) {
    if (ActiveWorld()==world) {
        ActiveWorld = 0;
    }
    WorldClosed(world);
    auto it = std::find(worlds.begin(), worlds.end(), world);
    worlds.erase(it);
    delete world;
}
