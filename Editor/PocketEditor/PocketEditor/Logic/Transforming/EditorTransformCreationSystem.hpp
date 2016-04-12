//
//  EditorTransformCreationSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 10/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "EditorTransform.hpp"
#include "Transform.hpp"
#include "Selectable.hpp"
#include "SelectableFactory.hpp"

using namespace Pocket;

class EditorTransformCreationSystem : public GameSystem<Transform> {
public:
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void SetEditorWorld(Pocket::GameWorld *editorWorld);
private:
    GameWorld* editorWorld;
    SelectableFactory* factory;
};
