//
//  EditorSelectionSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 06/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Sizeable.hpp"
#include "Selectable.hpp"
#include <map>

using namespace Pocket;

class EditorSizeableSystem : public GameSystem<Sizeable, Selectable> {
public:
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
private:
    void SelectionChanged(GameObject* object);
    void TryRemoveTransformObject(GameObject* object);
    using TransformObjects = std::map<GameObject*, GameObject*>;
    struct DraggerEvent { GameObject* dragger; GameObject* object;
        inline bool operator !=(const DraggerEvent& other) const{
            return dragger != other.dragger || object != other.object;
        }
    };
    void DraggerPositionChanged(DraggerEvent e);
    TransformObjects transformObjects;
};
