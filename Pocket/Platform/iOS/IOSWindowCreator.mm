//
//  IOSWindowCreator.cpp
//  Nano
//
//  Created by Jeppe Nielsen on 3/10/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "IOSWindowCreator.h"
#include "IOSWindow.h"

using namespace Nano;

IOSWindowCreator::IOSWindowCreator() {
    
    inputDevice.Initialize(12);
    isLandscape = false;
    
}

IOSWindowCreator::~IOSWindowCreator() {
    
}

void IOSWindowCreator::Create() {
    UIApplicationMain(0, nil, nil, NSStringFromClass([IOSWindow class]));
}

Nano::IOSWindowCreator* IOSWindowCreator::instance = 0;

Nano::IOSWindowCreator* IOSWindowCreator::Instance() {
    if (!instance) {
        instance = new IOSWindowCreator();
    }
    return instance;
}

void IOSWindowCreator::Down(int hash, int x, int y) {
    int index = CreateIndex();
    hashToID[hash] = index;
    inputDevice.SetTouch(index, true, Vector2(x,y));
}

void IOSWindowCreator::Move(int hash, int x, int y) {
    HashToID::iterator it = hashToID.find(hash);
    if (it==hashToID.end()) return;
    inputDevice.SetTouchPosition(it->second, Vector2(x,y));
}

void IOSWindowCreator::Up(int hash, int x, int y) {
    HashToID::iterator it = hashToID.find(hash);
    if (it==hashToID.end()) return;
    inputDevice.SetTouch(it->second, false, Vector2(x,y));
    hashToID.erase(it);
}

int IOSWindowCreator::CreateIndex() {
    if (hashToID.size()==0) return 0;
    int foundIndex = 0;
    while (true) {
        if (!IndexExists(foundIndex)) break;
        foundIndex++;
    }
    return foundIndex;
}

bool IOSWindowCreator::IndexExists(int index) {
    for (HashToID::iterator it = hashToID.begin(); it!=hashToID.end(); ++it) {
        if (it->second == index) return true;
    }
    return false;
}


