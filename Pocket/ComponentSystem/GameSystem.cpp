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

IGameSystem::IGameSystem() : world(0) {}
IGameSystem::~IGameSystem() {}

void IGameSystem::TryAddComponentContainer(ComponentID id, std::function<IContainer *()> constructor) {
    if (!world->components[id]) {
        world->components[id] = constructor();
    }
}

void IGameSystem::Initialize() {}
void IGameSystem::ObjectAdded(Pocket::GameObject *object) {}
void IGameSystem::ObjectRemoved(Pocket::GameObject *object) {}
void IGameSystem::Update(float dt) {}
void IGameSystem::Render() {}
const ObjectCollection& IGameSystem::Objects() const { return objects; }