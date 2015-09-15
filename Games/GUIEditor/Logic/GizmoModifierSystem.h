//
//  GizmoModifierSystem.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 14/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Gizmo.h"
#include "Sizeable.hpp"
#include "PropertyListener.hpp"

using namespace Pocket;

SYSTEM(GizmoModifierSystem, Gizmo, Sizeable)
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
private:
    struct Corner {
        GameObject* gizmo;
        GameObject* corner;
        int cornerIndex;
        
        inline bool operator ==(const Corner &other) const {
            return !(gizmo!=other.gizmo || corner!=other.corner || cornerIndex!=other.cornerIndex);
        }
        
        inline bool operator !=(const Corner &other) const {
            return (gizmo!=other.gizmo || corner!=other.corner || cornerIndex!=other.cornerIndex);
        }
        
        inline bool operator <(const Corner &other) const {
            return gizmo<other.gizmo;
        }
    };

    PropertyListener<Corner> changedPositions;
};