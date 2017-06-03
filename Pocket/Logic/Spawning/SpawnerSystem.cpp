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
    object->GetComponent<Spawner>()->Source.SetRoot(root);
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
        spawner->Source.SetRoot(root);
        if (spawner->Source) {
            std::cout << "Spawned Source Transform.Position = " << spawner->Source()->GetComponent<Transform>()->Position() << std::endl;
            
            auto clone = o->CreateChildClone(spawner->Source);
            std::cout << "Spawned clone Transform.Position = " << clone->GetComponent<Transform>()->Position() << std::endl;
            clone->Parent = o->Root();
        }
    }
    spawningObjects.clear();
}
