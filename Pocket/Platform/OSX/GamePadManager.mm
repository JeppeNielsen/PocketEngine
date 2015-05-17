//
//  GamePadManager.cpp
//  QuizMaster
//
//  Created by Jeppe Nielsen on 3/3/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#import <IOKit/hid/IOHIDLib.h>
#import <Foundation/Foundation.h>

#include "GamePadManager.hpp"
#include <iostream>
#include <string>

const int LeftStickX = 48;
const int LeftStickY = 49;

const int RightStickX = 51;
const int RightStickY = 52;

using namespace Pocket;

void gamepadWasAdded(void* inContext, IOReturn inResult, void* inSender, IOHIDDeviceRef device) {
    GamePadManager* padManager = (GamePadManager*)inContext;
    padManager->AddGamePad((long)inSender);
}

void gamepadWasRemoved(void* inContext, IOReturn inResult, void* inSender, IOHIDDeviceRef device) {
    GamePadManager* padManager = (GamePadManager*)inContext;
    padManager->RemoveGamePad((long)inSender);
}

void gamepadAction(void* inContext, IOReturn inResult, void* inSender, IOHIDValueRef value) {
  IOHIDElementRef element = IOHIDValueGetElement(value);
  int elementValue = (int)IOHIDValueGetIntegerValue(value);
  
  //int tUsagePage = IOHIDElementGetUsagePage(element);
  int buttonType = IOHIDElementGetUsage(element);

  GamePadManager* padManager = (GamePadManager*)inContext;
  padManager->buttons[buttonType] = elementValue;
  //padManager->Print();
  
  int gamePadID = (long)inSender;
  
  GamePadButton button = (GamePadButton)buttonType;
  
  if (buttonType == LeftStickX || buttonType == LeftStickY) {
        Vector2 value = padManager->GetAnalogState(gamePadID, GamePadButton::LeftStick);
        if (buttonType == LeftStickX) {
            value.x = ((float)elementValue) / (1<<15);
        } else {
            value.y = ((float)elementValue) / (1<<15);
        }
        padManager->SetAnalogState(gamePadID, GamePadButton::LeftStick, value);
    } else if (buttonType == RightStickX || buttonType == RightStickY) {
        Vector2 value = padManager->GetAnalogState(gamePadID,GamePadButton::RightStick);
        if (buttonType == RightStickX) {
            value.x = ((float)elementValue) / (1<<16);
        } else {
            value.y = ((float)elementValue) / (1<<16);
        }
        padManager->SetAnalogState(gamePadID, GamePadButton::RightStick, value);
    } else if (
                button == GamePadButton::LeftTrigger ||
                button == GamePadButton::RightTrigger
        ) {
        padManager->SetButtonState(gamePadID, button, elementValue > 1);
        padManager->SetAnalogState(gamePadID, button, Vector2(elementValue / 255.0f,0));
    } else {
        padManager->SetButtonState(gamePadID, button, elementValue == 1);
    }
}

void GamePadManager::Initialize() {
  IOHIDManagerRef hidManager;
  hidManager = IOHIDManagerCreate( kCFAllocatorDefault, kIOHIDOptionsTypeNone);
  NSMutableDictionary* criterion = [[NSMutableDictionary alloc] init];
  [criterion setObject: [NSNumber numberWithInt: kHIDPage_GenericDesktop] forKey: (NSString*)CFSTR(kIOHIDDeviceUsagePageKey)];
  [criterion setObject: [NSNumber numberWithInt: kHIDUsage_GD_GamePad] forKey: (NSString*)CFSTR(kIOHIDDeviceUsageKey)];
 // [criterion setObject: [NSNumber numberWithInt: kHIDUsage_Undefined] forKey: (NSString*)CFSTR(kIOHIDDeviceUsageKey)];
  
  
  IOHIDManagerSetDeviceMatching(hidManager, (CFDictionaryRef)criterion);
  [criterion dealloc];
  IOHIDManagerRegisterDeviceMatchingCallback(hidManager, gamepadWasAdded, (void*)this);
  IOHIDManagerRegisterDeviceRemovalCallback(hidManager, gamepadWasRemoved, (void*)this);
  IOHIDManagerScheduleWithRunLoop(hidManager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
  IOReturn tIOReturn = IOHIDManagerOpen(hidManager, kIOHIDOptionsTypeNone);
  IOHIDManagerRegisterInputValueCallback(hidManager, gamepadAction, (void*)this);
  device = hidManager;
}

GamePadManager::GamePadManager() : device(0), gamePadIndexCounter(0) { }
GamePadManager::~GamePadManager() { Destroy(); }

void GamePadManager::Destroy() {
    if (device) {
        IOHIDManagerRef hidManager = (IOHIDManagerRef)device;
        IOHIDManagerClose(hidManager, kIOHIDOptionsTypeNone);
        device = 0;
    }
}

void GamePadManager::Print() {
    std::cout<<" ------"<<std::endl;
    for (Buttons::iterator it = buttons.begin(); it!=buttons.end(); ++it) {
       std::cout<<" [" << (it->first) << "] = " << it->second << std::endl;
    }
    std::cout<<" ------" <<std::endl;
}

void GamePadManager::SetButtonState(int gamePadID, GamePadButton button, bool down) {
    GamePad& gamePad = gamePads[gamePadID];
    
    if (down) {
        gamePad.buttonValues[button] = true;
    } else {
        GamePad::ButtonValues::iterator it = gamePad.buttonValues.find(button);
        if (it!=gamePad.buttonValues.end()) {
            gamePad.buttonValues.erase(it);
        }
    }
}

void GamePadManager::SetAnalogState(int gamePadID, GamePadButton button, Vector2 value) {
    GamePad& gamePad = gamePads[gamePadID];
    gamePad.analogValues[button] = value;
}

Vector2 GamePadManager::GetAnalogState(int gamePadID, GamePadButton button) {
    GamePad& gamePad = gamePads[gamePadID];
    return gamePad.analogValues[button];
}

void GamePadManager::Update() {
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

void GamePadManager::AddGamePad(int gamePadID) {
    addedGamePads.push_back(gamePadID);
}

void GamePadManager::RemoveGamePad(int gamePadID) {
    removedGamePads.push_back(gamePadID);
}

void GamePadManager::GamePadAdded(int gamePadID) {
    GamePads::iterator it = gamePads.find(gamePadID);
    GamePad* gamePad = 0;
    if (it==gamePads.end()) {
        gamePad = &gamePads[gamePadID];
        gamePad->index = gamePadIndexCounter;
        gamePadIndexCounter++;
    } else {
        gamePad = &it->second;
    }
    GamePadPluggedIn(gamePad->index);
}

void GamePadManager::GamePadRemoved(int gamePadID) {
    GamePads::iterator it = gamePads.find(gamePadID);
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
    /*
    const float epsilon = 0.1f;
    for (AnalogValues::iterator current = analogValues.begin(); current!=analogValues.end(); ++current) {
        Vector2 delta = current->second;
        if (delta.Length()>epsilon) {
            delta.y=-delta.y;
            manager.AnalogChanged({ index, current->first, delta });
        }
    }
    */
    
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
    
    return;
    
    
    for (AnalogValues::iterator current = analogValues.begin(); current!=analogValues.end(); ++current) {
        AnalogValues::iterator prev = previousAnalogValues.find(current->first);
        Vector2 previousValue = prev == previousAnalogValues.end() ? Vector2(0,0) : prev->second;
        Vector2 delta = current->second - previousValue;
        if (delta.Length()>epsilon) {
            manager.AnalogChanged({ index, current->first, Vector2(current->second.x, -current->second.y) });
        } else if (current->second.Length()<=epsilon){
            manager.AnalogChanged({ index, current->first, Vector2(0,0) });
            
        }
    }
    previousAnalogValues = analogValues;
    
}




