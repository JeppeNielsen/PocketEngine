//
//  OpenWorldCollection.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 13/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "OpenWorldCollection.hpp"
#include "Project.hpp"
#include <fstream>

OpenWorldCollection::OpenWorldCollection() {
    ActiveWorld = 0;
}

OpenWorldCollection::~OpenWorldCollection() {
    Clear();
}

void OpenWorldCollection::Clear() {
    for(auto w : worlds) {
        w->Close();
        delete w;
    }
    worlds.clear();
    ActiveWorld = 0;
}

bool OpenWorldCollection::TryFindOpenWorld(const std::string &path, OpenWorld** world) {
    for(auto w : worlds) {
        if (w->Path == path) {
            *world = w;
            return true;
        }
    }
    *world = 0;
    return false;
}

OpenWorld* OpenWorldCollection::LoadWorld(const std::string &path, const std::string& filename, GameWorld& world, ScriptWorld& scriptWorld) {
    OpenWorld* openWorld;
    if (!TryFindOpenWorld(path, &openWorld)) {
        openWorld = new OpenWorld();
        openWorld->Load(path, filename, world, scriptWorld);
        worlds.push_back(openWorld);
        WorldLoaded(openWorld);
    }
    ActiveWorld = openWorld;
    return openWorld;
}

void OpenWorldCollection::CloseWorld(OpenWorld *world) {
    if (ActiveWorld()==world) {
        ActiveWorld = 0;
    }
    WorldClosed(world);
    auto it = std::find(worlds.begin(), worlds.end(), world);
    worlds.erase(it);
    world->Close();
    delete world;
}
