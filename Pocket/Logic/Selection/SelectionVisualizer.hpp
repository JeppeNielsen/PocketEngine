//
//  SelectionVisualizer.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 12/26/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Selectable.hpp"
#include <map>

namespace Pocket {
    class SelectionVisualizer : public GameSystem<Transform, Mesh, Selectable>  {
    public:
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        
    private:
        void SelectionChanged(GameObject* object);
        GameObject* CreateSelection(GameObject* object);
        
        typedef std::map<GameObject*, GameObject*> Selections;
        Selections selections;
    };
}