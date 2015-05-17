//
//  ClickSelectorSystem.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 3/26/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "ClickSelectorSystem.hpp"

using namespace Pocket;

void ClickSelectorSystem::Initialize() {
    AddComponent<Transform>();
    AddComponent<Selectable>();
    AddComponent<Touchable>();
}

void ClickSelectorSystem::AddedToWorld(Pocket::GameWorld &world) {
    selectables = world.GetSystem<SelectableCollection>();
    if (!selectables) {
        selectables = world.CreateSystem<SelectableCollection>();
    }
}

void ClickSelectorSystem::ObjectAdded(GameObject *object) {
    object->GetComponent<Touchable>()->Click += event_handler(this, &ClickSelectorSystem::TouchableClick, object);
}

void ClickSelectorSystem::ObjectRemoved(GameObject *object) {
    object->GetComponent<Touchable>()->Click -= event_handler(this, &ClickSelectorSystem::TouchableClick, object);
}

void ClickSelectorSystem::TouchableClick(Pocket::TouchData d, GameObject* object) {
    selectables->ClearSelection();
    object->GetComponent<Selectable>()->Selected = true;
}

