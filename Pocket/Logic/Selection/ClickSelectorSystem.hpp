//
//  ClickSelectorSystem.hpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 3/26/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "SelectableCollection.hpp"
#include "Touchable.hpp"

namespace Pocket {
    class ClickSelectorSystem : public GameSystem<Transform, Selectable, Touchable> {
    public:
        
        void Initialize();
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        static void CreateSubSystems(GameStorage& storage);
    private:
        SelectableCollection<Transform>* selectables;
        void TouchableClick(TouchData d, GameObject* object);
    };
}
