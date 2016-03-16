//
//  HierarchyOrder.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/12/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Orderable.hpp"
#include <set>

namespace Pocket {
    class HierarchyOrder : public GameSystem<Orderable> {
    public:
        void Initialize(GameWorld* world);
        void ObjectAdded(GameObject *object);
        void ObjectRemoved(GameObject *object);
        void SetDirty();
        void Update(float dt);
        void CalculateOrder(int& orderOffset, GameObject *object);
    private:
        bool orderIsDirty;
        GameWorld* world;
    };
}