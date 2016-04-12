//
//  SelectableFactory.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 11/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "SelectableFactory.hpp"


void SelectableFactory::Initialize(Pocket::GameWorld *world) {
    this->world = world;
}

GameObject* SelectableFactory::AddSelectable(Pocket::GameObject *objectInGame) {
    
    auto it = selectables.find(objectInGame);
    
    if (it==selectables.end()) {
        GameObject* selectableObject = world->CreateObject();
        selectableObject->AddComponent<Selectable>();
        auto& object = selectables[objectInGame];
        object.references = 1;
        object.object = selectableObject;
        return selectableObject;
    } else {
        ++it->second.references;
        return it->second.object;
    }
}

void SelectableFactory::RemoveSelectable(Pocket::GameObject *objectInGame) {
    auto it = selectables.find(objectInGame);
    --it->second.references;
    if (it->second.references==0) {
        it->second.object->Remove();
        selectables.erase(it);
    }
}

Selectable* SelectableFactory::GetSelectable(Pocket::GameObject *objectInGame) {
    auto it = selectables.find(objectInGame);
    if (it==selectables.end()) {
        return 0;
    } else {
        return it->second.object->GetComponent<Selectable>();
    }
}

GameObject* SelectableFactory::GetGameObject(Pocket::GameObject *selectedObject) {
    for(auto o : selectables) {
        if (o.second.object->GetComponent<Selectable>() == selectedObject->GetComponent<Selectable>()) {
            return o.first;
        }
    }
    return 0;
}
