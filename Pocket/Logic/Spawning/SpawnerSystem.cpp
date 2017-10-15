//
//  SpawnerSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 02/06/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "SpawnerSystem.hpp"
#include "Transform.hpp"

using namespace Pocket;

void SpawnerSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Spawner>()->Spawn.Changed.Bind(this, &SpawnerSystem::SpawnChanged, object);
}

void SpawnerSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Spawner>()->Spawn.Changed.Unbind(this, &SpawnerSystem::SpawnChanged, object);
    while (true) {
        auto it = std::find(spawningObjects.begin(), spawningObjects.end(), object);
        if (it!=spawningObjects.end()) {
            spawningObjects.erase(it);
        } else {
            break;
        }
    }
}

void SpawnerSystem::SpawnChanged(Pocket::GameObject *object) {
    Spawner* spawner = object->GetComponent<Spawner>();
    if (spawner->Spawn<=0) return;
    for (int i=0; i<spawner->Spawn; ++i) {
        spawningObjects.push_back(object);
    }
    spawner->Spawn = 0;
}

void SpawnerSystem::Update(float dt) {
    for(auto o : spawningObjects) {
        Spawner* spawner = o->GetComponent<Spawner>();
        if (spawner->Source) {
            spawner->Source()->Enabled = false;
            auto clone = o->CreateChildClone(spawner->Source);
            clone->Parent = o->Root();
        }
    }
    spawningObjects.clear();
}
