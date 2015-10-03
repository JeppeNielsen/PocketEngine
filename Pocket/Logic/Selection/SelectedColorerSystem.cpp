//
//  SelectedColorerSystem.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 03/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "SelectedColorerSystem.hpp"

using namespace Pocket;

void SelectedColorerSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed += event_handler(this, &SelectedColorerSystem::SelectedChanged, object);
    SelectedChanged(object->GetComponent<Selectable>(), object);
}

void SelectedColorerSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed -= event_handler(this, &SelectedColorerSystem::SelectedChanged, object);
}

void SelectedColorerSystem::SelectedChanged(Pocket::Selectable *selectable, Pocket::GameObject *object) {
    SelectedColorer* selectedColorer = object->GetComponent<SelectedColorer>();
    object->GetComponent<Colorable>()->Color = selectable->Selected() ? selectedColorer->Selected : selectedColorer->Deselected;
}
