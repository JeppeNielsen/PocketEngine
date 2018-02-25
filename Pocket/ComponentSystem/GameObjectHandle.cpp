//
//  GameObjectHandle.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 12/11/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameObjectHandle.hpp"
#include "GameStorage.hpp"

using namespace Pocket;

GameObjectHandle::GameObjectHandle() : world(0), index(-1) {}

GameObjectHandle::GameObjectHandle(const GameObjectHandle& other) : world(0), index(-1) { operator=(other); }

GameObjectHandle::GameObjectHandle(GameObject* object) : world(0), index(-1) { operator=(object); }

GameObjectHandle::~GameObjectHandle() {
    SetWorld(0);
}

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

void GameObjectHandle::Set(const Pocket::GameObject *ptr) {
    SetWorld(ptr->World());
    index = ptr->index;
    rootId = ptr->id;
    sceneGuid = ptr->scene->guid;
}

GameObject* GameObjectHandle::Get() {
    if (!world) return nullptr;
    
    if (index>=0 && index<world->storage->objects.entries.size() && world->storage->objects.versions[index] == version) {
        GameObject* ptr = &world->storage->objects.entries[index];
        if (ptr->id == rootId) return ptr;
    }
    
    GameObject* prefab = world->storage->TryGetPrefab(sceneGuid, rootId);
    if (!prefab) return nullptr;
    Set(prefab);
    return prefab;
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
        auto it = std::find(this->world->handles.begin(), this->world->handles.end(), this);
        if (it!=this->world->handles.end()) {
            this->world->handles.erase(it);
        }
    }
    this->world = world;
    if (this->world) {
        this->world->handles.push_back(this);
    }
}

const std::string& GameObjectHandle::SceneGuid() { return sceneGuid; }

bool GameObjectHandle::HasRoot() { return world !=nullptr; }

 
GameObject* GameObjectHandleRetriever::Get(GameWorld* world, int index, int version, int rootId, std::string sceneGuid) {
    GameObjectHandle handle;
    handle.SetWorld(world);
    handle.index = index;
    handle.version = version;
    handle.rootId = rootId;
    handle.sceneGuid = sceneGuid;
    return handle();
}

