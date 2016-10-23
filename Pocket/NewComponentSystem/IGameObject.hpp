//
//  IGameObject.hpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 18/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameIdHelper.hpp"

namespace Pocket {
    class GameObject;
    struct IGameObject {
        virtual bool HasComponent(ComponentId id) = 0;
        virtual void* GetComponent(ComponentId id) = 0;
        virtual void AddComponent(ComponentId id) = 0;
        virtual void AddComponent(ComponentId id, GameObject* referenceObject) = 0;
        virtual void RemoveComponent(ComponentId id) = 0;
        virtual void CloneComponent(ComponentId id, GameObject* object) = 0;
    };
}