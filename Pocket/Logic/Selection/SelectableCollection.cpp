//
//  SelectionCollection.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 3/26/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "SelectableCollection.hpp"

using namespace Pocket;

void SelectableCollection::Initialize(GameWorld* world) {
    hasChanged = false;
}

void SelectableCollection::ClearSelection() {
    for (auto o : Objects()) {
        o->GetComponent<Selectable>()->Selected = false;
    }
    hasChanged = true;
}

void SelectableCollection::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed.Bind(this, &SelectableCollection::SelectedChanged, object);
    SelectedChanged(object);
}

void SelectableCollection::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed.Unbind(this, &SelectableCollection::SelectedChanged, object);
    RemoveObject(object);
}

void SelectableCollection::SelectedChanged(GameObject* object) {
    Selectable* selectable = object->GetComponent<Selectable>();
    if (selectable->Selected) {
        selectedObjects.push_back(object);
        hasChanged = true;
    } else {
       RemoveObject(object);
    }
}

void SelectableCollection::RemoveObject(Pocket::GameObject *object) {
    auto i = std::find(selectedObjects.begin(), selectedObjects.end(), object);
    if (i!=selectedObjects.end()) {
        selectedObjects.erase(i);
        hasChanged = true;
    }
}

const IGameSystem::ObjectCollection& SelectableCollection::Selected() { return selectedObjects; }

void SelectableCollection::Update(float dt) {
    if (hasChanged) {
        hasChanged = false;
        SelectionChanged(this);
    }
}
