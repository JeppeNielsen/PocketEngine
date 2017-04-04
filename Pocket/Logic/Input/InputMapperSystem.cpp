//
//  InputMapperSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 10/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "InputMapperSystem.hpp"
#include "GameSystem.hpp"
#include <set>

using namespace Pocket;

void InputMapperSystem::Initialize() {
    root->Input().ButtonDown.Bind(this, &InputMapperSystem::ButtonDown);
    root->Input().ButtonUp.Bind(this, &InputMapperSystem::ButtonUp);
}

void InputMapperSystem::Update(float dt) {
    for(auto o : Objects()) {
        InputMapper* mapper = o->GetComponent<InputMapper>();
        
        std::set<std::string> downButtons;
        for (auto& button : buttons) {
            auto it = mapper->map.find(button);
            if (it == mapper->map.end()) continue;
            downButtons.insert(it->second);
        }
        InputController* controller = o->GetComponent<InputController>();
        controller->downButtons.clear();
        for(auto& s : downButtons) {
            controller->downButtons.push_back(s);
        }
        controller->InvokeEvents();
    }
}

void InputMapperSystem::ButtonDown(ButtonEvent e) {
    buttons.push_back(e.Id);
}

void InputMapperSystem::ButtonUp(ButtonEvent e) {
    auto it = std::find(buttons.begin(), buttons.end(), e.Id);
    if (it!=buttons.end()) {
        buttons.erase(it);
    }
}
