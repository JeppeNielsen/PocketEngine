//
//  SelectionCollection.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 3/26/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "SelectableCollection.hpp"

using namespace Pocket;

void SelectableCollection::Initialize() {
    AddComponent<Selectable>();
    AddComponent<Transform>();
}

void SelectableCollection::ClearSelection() {
    for (ObjectCollection::const_iterator it=Objects().begin(); it!=Objects().end(); ++it) {
        (*it)->GetComponent<Selectable>()->Selected = false;
    }
}

void SelectableCollection::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed += event_handler(this, &SelectableCollection::SelectedChanged, object);
    SelectedChanged(object->GetComponent<Selectable>(), object);
}

void SelectableCollection::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed -= event_handler(this, &SelectableCollection::SelectedChanged, object);
    RemoveObject(object);
}

void SelectableCollection::SelectedChanged(Pocket::Selectable *selectable, GameObject* object) {
    if (selectable->Selected) {
        selectedObjects.push_back(object);
    } else {
       RemoveObject(object);
    }
}

void SelectableCollection::RemoveObject(Pocket::GameObject *object) {
    auto i = std::find(selectedObjects.begin(), selectedObjects.end(), object);
    if (i!=selectedObjects.end()) {
        selectedObjects.erase(i);
    }
}

const ObjectCollection& SelectableCollection::Selected() { return selectedObjects; }