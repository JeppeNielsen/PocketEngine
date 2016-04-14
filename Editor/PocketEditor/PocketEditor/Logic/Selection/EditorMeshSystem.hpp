//
//  EditorSelectionSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 06/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Selectable.hpp"
#include <map>

using namespace Pocket;

class EditorMeshSystem : public GameSystem<Transform, Mesh, Selectable> {
public:
    void Initialize(GameWorld* world);
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
private:
    void SelectionChanged(GameObject* object);
    void TryRemoveTransformObject(GameObject* object);
    using TransformObjects = std::map<GameObject*, GameObject*>;
    TransformObjects transformObjects;
    GameWorld* world;
};