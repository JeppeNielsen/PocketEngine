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

GameObjectHandle::GameObjectHandle() : storage(nullptr), index(-1) {}

GameObjectHandle::GameObjectHandle(const GameObjectHandle& other) : storage(nullptr), index(-1) { operator=(other); }

GameObjectHandle::GameObjectHandle(GameObject* object) : storage(nullptr), index(-1) { operator=(object); }

GameObjectHandle::~GameObjectHandle() {
    SetStorage(nullptr);
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
    SetStorage(handle.storage);
    index = handle.index;
    version = handle.version;
    rootId = handle.rootId;
    sceneGuid = handle.sceneGuid;
}

void GameObjectHandle::Set(const Pocket::GameObject *ptr) {
    SetStorage(ptr->scene->storage);
    index = ptr->index;
    rootId = ptr->id;
    sceneGuid = ptr->scene->guid;
    version = storage->objects.versions[index];
}

GameObject* GameObjectHandle::Get() {
    if (!storage) return nullptr;
    
    if (index>=0 && index<storage->objects.entries.size() && storage->objects.versions[index] == version) {
        GameObject* ptr = &storage->objects.entries[index];
        if (ptr->id == rootId) return ptr;
    }
    
    GameObject* prefab = storage->TryGetPrefab(sceneGuid, rootId);
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

void GameObjectHandle::SetStorage(Pocket::GameStorage* storage) {
    if (this->storage) {
        auto it = std::find(this->storage->handles.begin(), this->storage->handles.end(), this);
        if (it!=this->storage->handles.end()) {
            this->storage->handles.erase(it);
        }
    }
    this->storage = storage;
    if (this->storage) {
        this->storage->handles.push_back(this);
    }
}

const std::string& GameObjectHandle::SceneGuid() { return sceneGuid; }

GameObject* GameObjectHandleRetriever::Get(GameStorage* storage, int index, int version, int rootId, std::string sceneGuid) {
    GameObjectHandle handle;
    handle.SetStorage(storage);
    handle.index = index;
    handle.version = version;
    handle.rootId = rootId;
    handle.sceneGuid = sceneGuid;
    return handle();
}

void GameObjectHandleRetriever::AddHandle(Pocket::GameObjectHandle *handle) {
    this->storage->handles.push_back(handle);
}

void GameObjectHandleRetriever::RemoveHandle(Pocket::GameObjectHandle *handle) {
    auto it = std::find(this->storage->handles.begin(), this->storage->handles.end(), handle);
    if (it!=this->storage->handles.end()) {
        this->storage->handles.erase(it);
    }
}

