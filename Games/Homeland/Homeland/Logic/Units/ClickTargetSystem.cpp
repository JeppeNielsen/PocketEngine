//
//  ClickDestinationSystem.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 11/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "ClickTargetSystem.h"

void ClickTargetSystem::AddedToWorld(Pocket::GameWorld &world) {
    selectables = world.CreateSystem<SelectableCollection>();
    selectables->AddComponent<Movable>();
}

void ClickTargetSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Click += event_handler(this, &ClickTargetSystem::ObjectClicked);
}

void ClickTargetSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Click -= event_handler(this, &ClickTargetSystem::ObjectClicked);
}

void ClickTargetSystem::ObjectClicked(Pocket::TouchData d) {
    if (d.Index != 1) return;
    for(auto o : selectables->Objects()) {
        if (!o->GetComponent<Selectable>()->Selected()) continue;
        o->GetComponent<Movable>()->Target = d.WorldPosition;
    }
}