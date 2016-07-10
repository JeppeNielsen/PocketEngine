//
//  ComponentEnablerSystem.cpp
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 10/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "ComponentEnablerSystem.h"

ComponentEnablerSystem::ComponentEnablerSystem()  {

}

void ComponentEnablerSystem::Initialize() {
    CurrentState.Changed.Bind(this, &ComponentEnablerSystem::StateChanged);
}

void ComponentEnablerSystem::ObjectAdded(GameObject* object) {
    UpdateObject(object);
}

void ComponentEnablerSystem::StateChanged() {
    for(GameObject* go : Objects()) {
        UpdateObject(go);
    }
}

void ComponentEnablerSystem::UpdateObject(Pocket::GameObject *object) {
    std::string state = CurrentState;
    ComponentEnabler* enabler = object->GetComponent<ComponentEnabler>();
    //object->Enabled() =  c.id == state;
   // for(auto& c : enabler->components) {
     //   object->EnableComponent(c.component, c.id == state);
    //}
}