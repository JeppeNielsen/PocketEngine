//
//  GamePadManager.cpp
//  QuizMaster
//
//  Created by Jeppe Nielsen on 3/5/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

//
//  GamePadManager.cpp
//  QuizMaster
//
//  Created by Jeppe Nielsen on 3/3/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "GamePadManager.hpp"
#include <iostream>
#include <emscripten/html5.h>

const int LeftStickX = 48;
const int LeftStickY = 49;

const int RightStickX = 51;
const int RightStickY = 52;

using namespace Pocket;




GamePadManager::GamePadManager() : device(0), gamePadIndexCounter(0) { }
GamePadManager::~GamePadManager() { Destroy(); }

void GamePadManager::Destroy() {
    if (device) {
       
    }
}


EM_BOOL gamePadConnected(int eventType, const EmscriptenGamepadEvent *gamepadEvent, void *userData) {
    GamePadManager* gamePadManager = (GamePadManager*)userData;
    gamePadManager->AddGamePad(gamepadEvent->index);
    return 1;
}

EM_BOOL gamePadDisconnected(int eventType, const EmscriptenGamepadEvent *gamepadEvent, void *userData) {
    GamePadManager* gamePadManager = (GamePadManager*)userData;
    gamePadManager->RemoveGamePad(gamepadEvent->index);
    return 1;
}

void GamePadManager::Initialize() {

    emscripten_set_gamepadconnected_callback(this, true, gamePadConnected);
    emscripten_set_gamepaddisconnected_callback(this, true, gamePadDisconnected);
}

void GamePadManager::Print() {
    std::cout<<" ------"<<std::endl;
    for (Buttons::iterator it = buttons.begin(); it!=buttons.end(); ++it) {
       std::cout<<" [" << (it->first) << "] = " << it->second << std::endl;
    }
    std::cout<<" ------" <<std::endl;
}

void GamePadManager::SetButtonState(int gamePadPtr, GamePadButton button, bool down) {
    GamePad& gamePad = gamePads[gamePadPtr];
    
    if (down) {
        gamePad.buttonValues[button] = true;
    } else {
        GamePad::ButtonValues::iterator it = gamePad.buttonValues.find(button);
        if (it!=gamePad.buttonValues.end()) {
            gamePad.buttonValues.erase(it);
        }
    }
}

void GamePadManager::SetAnalogState(int gamePadPtr, GamePadButton button, Vector2 value) {
    GamePad& gamePad = gamePads[gamePadPtr];
    gamePad.analogValues[button] = value;
}

Vector2 GamePadManager::GetAnalogState(int gamePadPtr, GamePadButton button) {
    GamePad& gamePad = gamePads[gamePadPtr];
    return gamePad.analogValues[button];
}

void GamePadManager::Update() {

    for (GamePads::iterator it = gamePads.begin(); it!=gamePads.end(); ++it) {
    
        EmscriptenGamepadEvent event;
        emscripten_get_gamepad_status(it->first, &event);
    
        for (int i=0; i<event.numButtons; i++) {
            SetButtonState(it->first, (GamePadButton)(i+1), event.digitalButton[i]);
        }

        if (event.numAxes>=2) {
            SetAnalogState(it->first, GamePadButton::LeftStick, Vector2(event.axis[0], -event.axis[1]));
        }
        
        if (event.numAxes>=4) {
            SetAnalogState(it->first, GamePadButton::RightStick, Vector2(event.axis[2], -event.axis[3]));
        }
    }

    UpdateAddedRemovedGamePads();
    UpdateButtons();
    UpdateAnalogs();
}

void GamePadManager::UpdateButtons() {
    for (GamePads::iterator it=gamePads.begin(); it!=gamePads.end(); ++it) {
        it->second.UpdateButtons(*this);
    }
}

void GamePadManager::UpdateAnalogs() {
    for (GamePads::iterator it=gamePads.begin(); it!=gamePads.end(); ++it) {
        it->second.UpdateAnalogs(*this);
    }
}

void GamePadManager::UpdateAddedRemovedGamePads() {
    for (int i=0; i<addedGamePads.size(); i++) {
        GamePadAdded(addedGamePads[i]);
    }
    addedGamePads.clear();
    
    for (int i=0; i<removedGamePads.size(); i++) {
        GamePadRemoved(removedGamePads[i]);
    }
    removedGamePads.clear();
}

void GamePadManager::AddGamePad(int gamePad) {
    addedGamePads.push_back(gamePad);
}

void GamePadManager::RemoveGamePad(int gamePad) {
    removedGamePads.push_back(gamePad);
}

void GamePadManager::GamePadAdded(int gamePadPtr) {
    GamePads::iterator it = gamePads.find(gamePadPtr);
    GamePad* gamePad = 0;
    if (it==gamePads.end()) {
        gamePad = &gamePads[gamePadPtr];
        gamePad->index = gamePadIndexCounter;
        gamePadIndexCounter++;
    } else {
        gamePad = &it->second;
    }
    GamePadPluggedIn(gamePad->index);
}

void GamePadManager::GamePadRemoved(int gamePadPtr) {
    GamePads::iterator it = gamePads.find(gamePadPtr);
    if (it!=gamePads.end()) {
       GamePadUnplugged(it->second.index);
    }
}

void GamePadManager::GamePad::UpdateButtons(Pocket::GamePadManager &manager) {

    for (ButtonValues::iterator it = buttonValues.begin(); it != buttonValues.end(); ++it) {
        bool isDown = previousButtonValues.find(it->first)==previousButtonValues.end();
        if (isDown) {
            manager.ButtonDown({ index, it->first, Vector2(0,0) });
        }
    }
    
    for (ButtonValues::iterator it = previousButtonValues.begin(); it!=previousButtonValues.end(); ++it) {
        bool isUp = buttonValues.find(it->first)==buttonValues.end();
        if (isUp) {
            manager.ButtonUp({ index, it->first, Vector2(0,0) });
        }
    }
    previousButtonValues = buttonValues;
}

void GamePadManager::GamePad::UpdateAnalogs(GamePadManager& manager) {
    std::vector<GamePadButton> remove;
    const float epsilon = 0.25f;
    for (AnalogValues::iterator current = analogValues.begin(); current!=analogValues.end(); ++current) {
        if (current->second.Length()>epsilon) {
            manager.AnalogChanged({ index, current->first, Vector2(current->second.x, -current->second.y) });
        } else {
            manager.AnalogChanged({ index, current->first, Vector2(0,0) });
            remove.push_back(current->first);
        }
    }
    for (int i=0; i<remove.size(); i++) {
        analogValues.erase(analogValues.find(remove[i]));
    }
}
