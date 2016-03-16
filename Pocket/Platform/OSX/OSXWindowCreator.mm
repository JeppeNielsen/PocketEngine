//
//  IOSWindowCreator.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/10/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "OSXWindowCreator.h"
#include "AppDelegate.h"

using namespace Pocket;

OSXWindowCreator::OSXWindowCreator() {
    
    inputDevice.Initialize(12);
    inputDevice.KeyboardChanged.Bind(this, &OSXWindowCreator::KeyboardChanged);
    
    isKeyboardActive = false;
}

OSXWindowCreator::~OSXWindowCreator() {
    
}

void OSXWindowCreator::Create() {
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    [NSApplication sharedApplication];
    
    AppDelegate *appDelegate = [[AppDelegate alloc] init];
    [NSApp setDelegate:appDelegate];
      [NSApp
        setPresentationOptions:NSFullScreenWindowMask];
    [NSApp run];
    [pool release];
    
     
    //(0, nil, nil, NSStringFromClass([IOSWindow class]));
}

Pocket::OSXWindowCreator* OSXWindowCreator::instance = 0;

Pocket::OSXWindowCreator* OSXWindowCreator::Instance() {
    if (!instance) {
        instance = new OSXWindowCreator();
    }
    return instance;
}

void OSXWindowCreator::Down(int index, int x, int y) {
    inputDevice.SetTouch(index, true, Vector2(x,y));
}

void OSXWindowCreator::Move(int index, int x, int y) {
    inputDevice.SetTouchPosition(index, Vector2(x,y));
}

void OSXWindowCreator::Up(int index, int x, int y) {
    inputDevice.SetTouch(index, false, Vector2(x,y));
}

void OSXWindowCreator::Set(int index, int x, int y) {
    inputDevice.SetTouch(index, false, Vector2(x,y));
}

void OSXWindowCreator::SetScroll(float delta) {
    inputDevice.SetScroll(delta);
}

void OSXWindowCreator::ButtonDown(const std::string& button) {
    inputDevice.SetButton(button, true);

    if (isKeyboardActive) {
        
        //backspace
        if (button == "\x7f") {
            inputDevice.KeyboardText = inputDevice.KeyboardText.substr(0, inputDevice.KeyboardText.length() - 1);
        }//escape or return
        else if (button == "\r" || button == "\e") {
            inputDevice.KeyboardActive = false;
        } // tab
        else if (button == "\t") {
            inputDevice.KeyboardActive = false;
        }
        else {
            inputDevice.KeyboardText += button;
        }
    }
}

void OSXWindowCreator::ButtonUp(const std::string& button) {
    inputDevice.SetButton(button, false);
}

void OSXWindowCreator::KeyboardChanged(InputDevice::KeyboardEventData keyboardData) {
    inputDevice.KeyboardActive = keyboardData.active;
    isKeyboardActive = keyboardData.active;
    inputDevice.KeyboardText = keyboardData.text;
}

