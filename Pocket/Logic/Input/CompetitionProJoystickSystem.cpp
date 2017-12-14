//
//  CompetitionProJoystickSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 28/11/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "CompetitionProJoystickSystem.hpp"

using namespace Pocket;

void CompetitionProJoystickSystem::Update(float dt) {
    for(auto o : Objects()) {
        
    
        HidDevice* device = o->GetComponent<HidDevice>();
        CompetitionProJoystick* joystick = o->GetComponent<CompetitionProJoystick>();
        
        if (device->data.size()!=20) {
            device->data.resize(20);
            device->prevData.resize(20);
        }
        
        bool buttonIsDown = device->data[2]>0;
        bool buttonWasDown = device->prevData[2]>0;
        
        if (buttonIsDown && !buttonWasDown) {
            joystick->ButtonDown(0);
        } else if (!buttonIsDown && buttonWasDown) {
            joystick->ButtonUp(0);
        }
    }
}
