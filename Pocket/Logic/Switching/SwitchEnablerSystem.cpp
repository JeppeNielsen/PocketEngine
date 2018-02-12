//
//  SwitchEnablerSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 22/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "SwitchEnablerSystem.hpp"


using namespace Pocket;

void SwitchEnablerSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Switch>()->On.Changed.Bind(this, &SwitchEnablerSystem::SwitchChanged, object);
    SwitchChanged(object);
}

void SwitchEnablerSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Switch>()->On.Changed.Unbind(this, &SwitchEnablerSystem::SwitchChanged, object);
}

void SwitchEnablerSystem::SwitchChanged(Pocket::GameObject *object) {
    bool on = object->GetComponent<Switch>()->On;
    for(auto child : object->Hierarchy().Children()) {
        child->Hierarchy().Enabled = on;
    }
}
