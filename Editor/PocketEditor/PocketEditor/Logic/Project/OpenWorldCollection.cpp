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
#include "EditorContext.hpp"

OpenWorldCollection::OpenWorldCollection() {
    ActiveWorld = 0;
}

OpenWorldCollection::~OpenWorldCollection() {
    Clear();
}

void OpenWorldCollection::Clear() {
    auto temp = worlds;
    for(auto w : temp) {
        CloseWorld(w);
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

OpenWorld* OpenWorldCollection::LoadWorld(const std::string &path, const std::string& filename, EditorContext* context) {
    OpenWorld* openWorld;
    if (!TryFindOpenWorld(path, &openWorld)) {
        openWorld = new OpenWorld();
        if (!openWorld->Load(path, filename, context)) {
            delete openWorld;
            return 0;
        } else {
            worlds.push_back(openWorld);
            WorldLoaded(openWorld);
        }
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

void OpenWorldCollection::PreCompile() {
    for(auto world : worlds) {
        world->PreCompile();
    }
}

void OpenWorldCollection::PostCompile() {
    for(auto world : worlds) {
        world->PostCompile();
    }
}