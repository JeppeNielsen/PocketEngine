//
//  InputMapperSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 10/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "InputMapperSystem.hpp"
#include "GameWorld.hpp"
#include <set>

using namespace Pocket;

void InputMapperSystem::Initialize() {
    world->Input().ButtonDown.Bind(this, &InputMapperSystem::ButtonDown);
    world->Input().ButtonUp.Bind(this, &InputMapperSystem::ButtonUp);
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

void InputMapperSystem::ButtonDown(std::string button) {
    buttons.push_back(button);
}

void InputMapperSystem::ButtonUp(std::string button) {
    auto it = std::find(buttons.begin(), buttons.end(), button);
    if (it!=buttons.end()) {
        buttons.erase(it);
    }
}