//
//  InputController.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 10/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "InputController.hpp"

using namespace Pocket;

bool InputController::IsButtonDown(std::string button) {
    auto it = std::find(downButtons.begin(), downButtons.end(), button);
    return it != downButtons.end();
}

void InputController::InvokeEvents() {
    for(auto& b : downButtons) {
        if (std::find(previouslyDownButtons.begin(), previouslyDownButtons.end(), b) == previouslyDownButtons.end()) {
            ButtonDown(b);
        }
    }
    
    for(auto& b : previouslyDownButtons) {
        if (std::find(downButtons.begin(), downButtons.end(), b) == downButtons.end()) {
            ButtonUp(b);
        }
    }
    previouslyDownButtons = downButtons;
}