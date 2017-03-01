//
//  TriggerTouchSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 25/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "TriggerTouchSystem.hpp"
#include "GameWorld.hpp"
#include <fstream>

using namespace Pocket;

void TriggerTouchSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Click.Bind(this, &TriggerTouchSystem::Clicked, object);
}

void TriggerTouchSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Click.Bind(this, &TriggerTouchSystem::Clicked, object);
}

void TriggerTouchSystem::Clicked(TouchData d, Pocket::GameObject *object) {
    Trigger* trigger = object->GetComponent<Trigger>();
    trigger->Invoke();
}
