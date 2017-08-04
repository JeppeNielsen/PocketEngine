//
//  PanelSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 04/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Panel.hpp"
#include "Transform.hpp"
#include "Sizeable.hpp"

namespace Pocket {
    struct PanelSystem : public GameSystem<Panel, Transform, Sizeable> {
        void ObjectAdded(GameObject* object) override;
        void ObjectRemoved(GameObject* object) override;
        void AreaChanged(GameObject* object);
        void InvokeAreaChanged(GameObject* object, GameObject* prev, GameObject* current);
        void AreaSizeChanged(GameObject* object);
        void SplitValueChanged(std::string id, GameObject* object);
    };
}
