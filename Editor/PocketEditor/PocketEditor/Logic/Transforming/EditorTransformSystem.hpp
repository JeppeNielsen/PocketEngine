//
//  EditorTransformSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 10/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "EditorTransform.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Selectable.hpp"

using namespace Pocket;

class EditorTransformSystem : public GameSystem<Transform, Selectable, EditorTransform> {
public:
    void Initialize(GameWorld* world);
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
private:
    void SelectionChanged(GameObject* object);

    GameWorld* world;
};