//
//  GameSystem.cpp
//  EntitySystem
//
//  Created by Jeppe Nielsen on 08/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameSystem.hpp"
#include "GameWorld.hpp"

using namespace Pocket;

GameSystemBase::GameSystemBase() : world(0) {}
GameSystemBase::~GameSystemBase() {}

void GameSystemBase::TryAddComponentContainer(ComponentID id, std::function<IContainer *(std::string&)>&& constructor) {
    world->TryAddComponentContainer(id, std::move(constructor));
}

void GameSystemBase::Initialize() {}
void GameSystemBase::ObjectAdded(Pocket::GameObject *object) {}
void GameSystemBase::ObjectRemoved(Pocket::GameObject *object) {}
void GameSystemBase::Update(float dt) {}
void GameSystemBase::Render() {}
const ObjectCollection& GameSystemBase::Objects() const { return objects; }

int GameSystemBase::AddObject(Pocket::GameObject *object) {
    int count = (int)objects.size();
    objects.push_back(object);
    return count;
}

void GameSystemBase::RemoveObject(Pocket::GameObject *object) {
    objects.erase(std::find(objects.begin(), objects.end(), object));
}

void GameSystemBase::SetMetaData(Pocket::GameObject *object, void *data) {
    metaData[object] = data;
}

void* GameSystemBase::GetMetaData(Pocket::GameObject *object) {
    return metaData[object];
}