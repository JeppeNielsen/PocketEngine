//
//  TriggerSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 25/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "TriggerSystem.hpp"
#include "GameWorld.hpp"
#include <fstream>

using namespace Pocket;

void TriggerSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Trigger>()->Source.Changed.Bind(this, &TriggerSystem::CloneSourceChanged, object);
    if (Objects().size() == 1) {
        variablesRoot = root->World()->CreateScene();
    }
    CloneSourceChanged(object);
}

void TriggerSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Trigger>()->Source.Changed.Unbind(this, &TriggerSystem::CloneSourceChanged, object);
    if (Objects().size() == 1) {
        variablesRoot->Remove();
    }
}

void TriggerSystem::CloneSourceChanged(Pocket::GameObject *object) {
    Trigger* trigger = object->GetComponent<Trigger>();
    trigger->CreateVariables(variablesRoot);
}
