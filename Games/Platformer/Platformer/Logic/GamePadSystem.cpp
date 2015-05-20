//
//  GamePadSystem.cpp
//  Platformer
//
//  Created by Jeppe Nielsen on 27/04/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "GamePadSystem.hpp"

void GamePadSystem::ObjectAdded(Pocket::GameObject *object) {
    
}


void GamePadSystem::ObjectRemoved(Pocket::GameObject *object) {

}

void GamePadSystem::SetInput(Pocket::InputManager *input) {
    this->input = input;
    this->input->GamePad.ButtonDown += event_handler(this, &GamePadSystem::ButtonDown);
    this->input->GamePad.ButtonUp += event_handler(this, &GamePadSystem::ButtonUp);
    this->input->GamePad.AnalogChanged += event_handler(this, &GamePadSystem::AnalogChanged);
}


void GamePadSystem::ButtonDown(Pocket::GamePadData d) {
    GameObject* g = FindObject(d.GamePad);
    if (!g) return;
    if (!g->GetComponent<Groundable>()->IsGrounded) return;
    g->GetComponent<CharacterController>()->movement += Vector2(0,40);
}

void GamePadSystem::ButtonUp(Pocket::GamePadData d) {
    
}

void GamePadSystem::AnalogChanged(Pocket::GamePadData d) {
    if (d.Button != GamePadButton::LeftStick) return;
    movement[d.GamePad] = d.Direction;
}

GameObject* GamePadSystem::FindObject(int index) {
    for (int i=0; i<Objects().size(); i++) {
        if (Objects()[i]->GetComponent<GamePadController>()->gamePadIndex == index) return Objects()[i];
    }
    return 0;
}

void GamePadSystem::Update(float dt) {
    for (Movement::iterator it = movement.begin(); it!=movement.end(); ++it) {
        GameObject* g = FindObject(it->first);
        if (!g) continue;
        g->GetComponent<CharacterController>()->movement += Vector2(it->second.x,0);
    }
    movement.clear();
}