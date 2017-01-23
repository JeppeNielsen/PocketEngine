//
//  TouchSwitchSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 21/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "TouchSwitchSystem.hpp"
using namespace Pocket;

void TouchSwitchSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Click.Bind(this, &TouchSwitchSystem::TouchClick, object);
}

void TouchSwitchSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Click.Unbind(this, &TouchSwitchSystem::TouchClick, object);
}

void TouchSwitchSystem::TouchClick(Pocket::TouchData d, Pocket::GameObject *object) {
    object->GetComponent<Switch>()->On = true;
}
