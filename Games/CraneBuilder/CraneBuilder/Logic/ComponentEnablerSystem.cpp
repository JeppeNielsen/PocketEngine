//
//  ComponentEnablerSystem.cpp
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 10/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "ComponentEnablerSystem.h"

ComponentEnablerSystem::ComponentEnablerSystem() : CurrentState(this) {

}

void ComponentEnablerSystem::AddedToWorld(Pocket::GameWorld &world) {
    CurrentState.Changed += event_handler(this, &ComponentEnablerSystem::StateChanged);
}

void ComponentEnablerSystem::ObjectAdded(GameObject* object) {
    UpdateObject(object);
}

void ComponentEnablerSystem::StateChanged(ComponentEnablerSystem *system) {
    for(GameObject* go : Objects()) {
        UpdateObject(go);
    }
}

void ComponentEnablerSystem::UpdateObject(Pocket::GameObject *object) {
    std::string state = CurrentState;
    ComponentEnabler* enabler = object->GetComponent<ComponentEnabler>();
    for(auto& c : enabler->components) {
        object->EnableComponent(c.component, c.id == state);
    }
}