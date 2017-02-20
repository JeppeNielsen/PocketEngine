//
//  GameObjectHandle.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 12/11/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameObjectHandle.hpp"

using namespace Pocket;

GameObjectHandle::GameObjectHandle() : world(0), index(-1) {}

GameObject* GameObjectHandle::operator->() {
    return Get();
}

GameObject* GameObjectHandle::operator()() {
    return Get();
}

GameObjectHandle::operator Pocket::GameObject * () {
    return Get();
}

void GameObjectHandle::operator=(const Pocket::GameObject &v) {
    Set(&v);
    Changed();
}

void GameObjectHandle::operator=(Pocket::GameObject *v) {
    Set(v);
    Changed();
}

void GameObjectHandle::operator=(const GameObjectHandle& handle) {
    SetWorld(handle.world);
    index = handle.index;
    version = handle.version;
    rootId = handle.rootId;
    sceneGuid = handle.sceneGuid;
}

void GameObjectHandle::SetRoot(const Pocket::GameObject* root) {
    SetWorld(root->scene->world);
}

void GameObjectHandle::Set(const Pocket::GameObject *ptr) {
    SetRoot(ptr);
    index = ptr->index;
    rootId = ptr->rootId;
    sceneGuid = ptr->scene->guid;
}

GameObject* GameObjectHandle::Get() {
    if (!world) return 0;
    
    if (index>=0 && index<world->objects.entries.size() && world->objects.versions[index] == version) {
        GameObject* ptr = &world->objects.entries[index];
        if (ptr->rootId == rootId) return ptr;
    }
    
    GameScene* scene = world->TryGetScene(sceneGuid);
    if (!scene) return 0;
    GameObject* foundObject = scene->FindObject(rootId);
    if (foundObject) {
        Set(foundObject);
        return foundObject;
    } else {
        return 0;
    }
}

GameObjectHandle GameObjectHandle::Deserialize(const std::string &data) {
    GameObjectHandle handle;
    size_t colonLocation = data.rfind(":");
    if (colonLocation!=-1) {
        handle.sceneGuid = data.substr(0, colonLocation);
        std::string objectIdStr = data.substr(colonLocation+1, data.size() - colonLocation-1);
        handle.rootId = ::atoi(objectIdStr.c_str());
    }
    return handle;
}

void GameObjectHandle::SetWorld(Pocket::GameWorld *world) {
    if (this->world) {
        this->world->handles.erase(std::find(this->world->handles.begin(), this->world->handles.end(), this));
    }
    this->world = world;
    if (this->world) {
        this->world->handles.push_back(this);
    }
}
