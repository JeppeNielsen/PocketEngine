//
//  SpawnerSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 02/06/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Spawner.hpp"

namespace Pocket {
    class SpawnerSystem : public GameSystem<Spawner> {
    protected:
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        void Update(float dt);
    private:
        void SpawnChanged(GameObject* object);
        std::vector<GameObject*> spawningObjects;
    };
}
