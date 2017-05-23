//
//  AppMenu.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 21/07/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "AppMenu.hpp"
#import <Cocoa/Cocoa.h>
#include <iostream>
#include <map>
#include "OSXView.h"
#include "OSXWindowCreator.h"
using namespace Pocket;

struct Data {
    Data() : menu(0), menuItem(0) {}
    NSMenu* menu;
    NSMenuItem* menuItem;
};

std::map<AppMenu*, Data> menuData;

AppMenu::AppMenu() : parent(0) { }
AppMenu::~AppMenu() {
    for(auto child : children) {
        delete child;
    }
    auto it = menuData.find(this);
    
    auto parentIt = menuData.find(parent);
    
    if (parentIt!=menuData.end()) {
        OSXView* osxView = (OSXView*)OSXWindowCreator::Instance()->win;
        
        [osxView removeMenuItem:parentIt->second.menu withItem:this];
    }
    
    if (it->second.menu && it->second.menu != [NSApp mainMenu]) {
        [it->second.menu release];
    }
    
    if (it->second.menuItem) {
        [it->second.menuItem release];
    }
    menuData.erase(it);
}

void AppMenu::InitializeMainMenu() {
    Data data;
    data.menu = [NSApp mainMenu];
    menuData[this] = data;
}

void AppMenu::InitializePopUp() {
    Data data;
    data.menu = [[NSMenu alloc] initWithTitle:@""];
    menuData[this] = data;
}

void AppMenu::CreateAppMenu(AppMenu* parent, const std::string& text, const std::string& shortcut) {
    this->text = text;
    this->parent = parent;
    
    NSString* textString = [[NSString alloc] initWithUTF8String:text.c_str()];
    NSString* shortCutString = [[NSString alloc] initWithUTF8String:shortcut.c_str()];

    Data data;
    
    OSXView* osxView = (OSXView*)OSXWindowCreator::Instance()->win;
    if (parent) {
        auto it = menuData.find(parent);
        if (!it->second.menu) {
            NSString* parentTextString = [[NSString alloc] initWithUTF8String:parent->text.c_str()];
            it->second.menu = [[[NSMenu alloc] initWithTitle:parentTextString] retain];
            [it->second.menuItem setSubmenu:it->second.menu];
        }
        
        data.menuItem = [osxView createMenuItem:it->second.menu withText:textString withObject:this withShortCut:shortCutString];
    }
    
    menuData[this] = data;
}

AppMenu& AppMenu::AddChild(const std::string &text, const std::string& shortcut) {
    AppMenu* menu = new AppMenu();
    menu->CreateAppMenu(this, text, shortcut);
    children.push_back( menu );
    return *children.back();
}

void AppMenu::ShowPopup(const Vector2& location) {
    NSMenu* menu = menuData[this].menu;
    NSPoint point;
    point.x = location.x;
    point.y = location.y;
    
    OSXView* osxView = (OSXView*)OSXWindowCreator::Instance()->win;
    
    point = [osxView convertViewLocationToWorldPoint:point];
    
    [menu popUpMenuPositioningItem:nil atLocation:point inView:nil];
}

void AppMenu::Clear() {
    for(auto child : children) {
        delete child;
    }
    children.clear();
}
