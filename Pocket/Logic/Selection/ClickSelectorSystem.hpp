//
//  ClickSelectorSystem.hpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 3/26/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "SelectableCollection.hpp"
#include "Touchable.hpp"

namespace Pocket {
    class ClickSelectorSystem : public GameSystem<Transform, Selectable, Touchable> {
    public:
        void Initialize(GameWorld* world);
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
    private:
        SelectableCollection<Transform>* selectables;
        void TouchableClick(TouchData d, GameObject* object);
    };
}