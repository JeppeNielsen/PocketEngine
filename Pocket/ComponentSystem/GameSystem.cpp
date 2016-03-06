//
//  IGameSystem.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 05/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameSystem.hpp"

using namespace Pocket;

IGameSystem::~IGameSystem() { }

const IGameSystem::ObjectCollection& IGameSystem::Objects() {
    return objects;
}

void IGameSystem::SetMetaData(GameObject* object, void* data) {
    metadata[object] = data;
}

void* IGameSystem::GetMetaData(GameObject* object) {
    return metadata[object];
}

void IGameSystem::Initialize(GameWorld* world) { }
void IGameSystem::Update(float dt) { }
void IGameSystem::Render() { }
void IGameSystem::ObjectAdded(GameObject* object) {}
void IGameSystem::ObjectRemoved(GameObject* object) {}