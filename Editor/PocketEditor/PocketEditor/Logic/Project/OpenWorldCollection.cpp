//
//  OpenWorldCollection.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 13/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "OpenWorldCollection.hpp"

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

OpenWorld* OpenWorldCollection::LoadWorld(const std::string &path) {
    OpenWorld* world = new OpenWorld();
    world->Path = path;
    worlds.push_back(world);
    world->CreateDefault(*input);
    ActiveWorld = world;
    return world;
}

void OpenWorldCollection::CloseWorld(OpenWorld *world) {
    if (ActiveWorld()==world) {
        ActiveWorld = 0;
    }
    auto it = std::find(worlds.begin(), worlds.end(), world);
    worlds.erase(it);
    delete world;
}

void OpenWorldCollection::Initialize(Pocket::InputManager& input) {
    this->input = &input;
}