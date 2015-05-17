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