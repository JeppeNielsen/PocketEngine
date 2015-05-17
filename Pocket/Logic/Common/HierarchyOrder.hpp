//
//  HierarchyOrder.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/12/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Orderable.hpp"
#include <set>

namespace Pocket {
    class HierarchyOrder : public GameSystem {
    public:
        void Initialize();
        void Update(float dt);
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        
        void ParentChanged(Property<GameObject*, GameObject*>::EventData d);
        void OrderChanged(GameObject* object);
        
    private:
        bool orderIsDirty;
        void CalculateOrder(int& order, GameObject* hierarchy);
    };
}