//
//  SelectionCollection.hpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 3/26/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Transform.hpp"
#include "Selectable.hpp"

namespace Pocket {
    class SelectableCollection : public GameSystem {
    public:
        void Initialize();
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        void Update(float dt);
        void ClearSelection();
        const ObjectCollection& Selected();
        Event<SelectableCollection*> SelectionChanged;
    private:
        void SelectedChanged(Selectable* selectable, GameObject* object);
        ObjectCollection selectedObjects;
        void RemoveObject(GameObject* object);
        bool hasChanged;
    };
}