//
//  GameSystem.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 9/29/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "GameSystem.hpp"
#include "GameObject.hpp"

using namespace Pocket;

GameSystem::~GameSystem() { }

GameWorld* GameSystem::World() { return world; }

void GameSystem::Initialize() {}

const ObjectCollection& GameSystem::Objects() { return objects; }

void GameSystem::ObjectAdded(GameObject *object) {}
void GameSystem::ObjectRemoved(GameObject *object) {}

void GameSystem::Update(float dt) {}
void GameSystem::Render() {}
void GameSystem::AddedToWorld(Pocket::GameWorld &world) {}

void GameSystem::SetMetaData(Pocket::GameObject *object, void *data) {
    metaData[object] = data;
    //if (!object->metaData) {
    //    object->metaData = new GameObject::MetaData[world->systems.size()];
    //}
    //object->metaData[id] = data;
}

void* GameSystem::GetMetaData(Pocket::GameObject *object) {
    //return object->metaData[id];
    return metaData[object];
}