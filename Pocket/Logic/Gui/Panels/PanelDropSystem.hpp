//
//  PanelDropSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 04/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Droppable.hpp"
#include "PanelDropper.hpp"
#include "Transform.hpp"

namespace Pocket {
    class PanelSystem;
    struct PanelDropSystem : public GameSystem<Droppable, PanelDropper, Transform> {
        PanelSystem* panels;
        void Initialize() override;
        void ObjectAdded(GameObject* object) override;
        void ObjectRemoved(GameObject* object) override;
        void Dropped(DroppedData d);
        static void CreateSubSystems(GameStorage& storage);
    };
}
