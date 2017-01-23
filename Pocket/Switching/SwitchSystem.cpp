//
//  SwitchSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 21/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "SwitchSystem.hpp"
#include <algorithm>
#include <map>

using namespace Pocket;

void SwitchSystem::Initialize() {
    isDirty = false;
    disableListening = false;
}

void SwitchSystem::ObjectAdded(GameObject *object) {
    Switch* sw = object->GetComponent<Switch>();
    sw->On.Changed.Bind(this, &SwitchSystem::SwitchChanged, object);
}

void SwitchSystem::ObjectRemoved(GameObject *object) {
    object->GetComponent<Switch>()->On.Changed.Unbind(this, &SwitchSystem::SwitchChanged, object);
    turnedOnSwitches.erase(std::remove_if(turnedOnSwitches.begin(), turnedOnSwitches.end(), [object](Switch* sw) {
        return sw == object->GetComponent<Switch>();
    }), turnedOnSwitches.end());
}

void SwitchSystem::SwitchChanged(Pocket::GameObject *object) {
    Switch* sw = object->GetComponent<Switch>();
    if (disableListening) return;
    if (sw->On) {
        turnedOnSwitches.push_back(sw);
        isDirty = true;
    }
}

void SwitchSystem::Update(float dt) {
    if (!isDirty) return;
    isDirty = false;
    
    std::map<int, Switch*> groupSwitchOn;
    for(auto sw : turnedOnSwitches) {
        groupSwitchOn[sw->Group] = sw;
    }
    
    disableListening = true;
    for(auto group : groupSwitchOn) {
        int groupId = group.first;
        Switch* turnedOnSwitch = group.second;
        for(auto o : Objects()) {
            Switch* sw = o->GetComponent<Switch>();
            if (o->GetComponentOwner<Switch>()!=o) continue;
            if (sw->Group()==groupId) {
                sw->On = (sw == turnedOnSwitch);
            }
        }
    }
    disableListening = false;
}
