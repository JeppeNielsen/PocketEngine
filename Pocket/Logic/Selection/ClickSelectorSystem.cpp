//
//  ClickSelectorSystem.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 3/26/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "ClickSelectorSystem.hpp"

using namespace Pocket;

void ClickSelectorSystem::CreateSubSystems(Pocket::GameStorage &storage) {
    storage.AddSystemType<SelectableCollection<Transform>>();
}

void ClickSelectorSystem::Initialize() {
    selectables = root->CreateSystem<SelectableCollection<Transform>>();
}

void ClickSelectorSystem::ObjectAdded(GameObject *object) {
    object->GetComponent<Touchable>()->Click.Bind(this, &ClickSelectorSystem::TouchableClick, object);
}

void ClickSelectorSystem::ObjectRemoved(GameObject *object) {
    object->GetComponent<Touchable>()->Click.Unbind(this, &ClickSelectorSystem::TouchableClick, object);
}

void ClickSelectorSystem::TouchableClick(Pocket::TouchData d, GameObject* object) {
    selectables->ClearSelection();
    object->GetComponent<Selectable>()->Selected = true;//!object->GetComponent<Selectable>()->Selected;
}
