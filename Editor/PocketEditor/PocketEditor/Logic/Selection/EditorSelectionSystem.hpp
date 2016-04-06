//
//  EditorSelectionSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 06/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "EditorSelection.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Selectable.hpp"

using namespace Pocket;

class EditorSelectionSystem : public GameSystem<Transform, Selectable, EditorSelection> {
public:
    void Initialize(GameWorld* world);
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
private:
    void SelectionChanged(GameObject* object);

    GameWorld* world;
};