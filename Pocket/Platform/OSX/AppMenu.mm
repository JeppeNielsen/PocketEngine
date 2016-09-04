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
    NSMenu* menu;
    NSMenuItem* menuItem;
};

std::map<AppMenu*, Data> menuData;

AppMenu::AppMenu() : parent(0) { }
AppMenu::~AppMenu() {
    for(auto child : children) {
        delete child;
    }
}
AppMenu::AppMenu(AppMenu* parent, const std::string& text, const std::string& shortcut) : text(text), parent(parent) {

/*


{
NSMenuItem* mi = [menu addItemWithTitle:@"" action:nil keyEquivalent:@""];
    NSMenu* appMenu = [[NSMenu alloc] initWithTitle:@"My App"];
        [appMenu addItemWithTitle:@"Item 1" action:nil keyEquivalent:@""];

    [mi setSubmenu:appMenu];
}



*/

    
    NSString* textString = [[NSString alloc] initWithUTF8String:text.c_str()];
    NSString* shortCutString = [[NSString alloc] initWithUTF8String:shortcut.c_str()];

    Data data;
    
    OSXView* osxView = (OSXView*)OSXWindowCreator::Instance()->win;

    if (!parent->parent) {
        NSMenu* menu = [NSApp mainMenu];
        //data.menuItem = [menu addItemWithTitle:@"" action:nil keyEquivalent:@""];
        data.menuItem = [osxView createMenuItem:menu withText:@"" withObject:this withShortCut:shortCutString];
        data.menu =  [[NSMenu alloc] initWithTitle:textString];
        [data.menuItem setSubmenu:data.menu];
        
    } else {
        auto it = menuData.find(parent);

        NSMenu* menu = it->second.menu;
    
        data.menuItem = [osxView createMenuItem:menu withText:textString withObject:this withShortCut:shortCutString]; //[menu addItemWithTitle:textString action:@selector(Selected) keyEquivalent:@""];
        //data.menu = [[NSMenu alloc] initWithTitle:textString];
        //data.menuItem = [data.menu addItemWithTitle:textString action:nil keyEquivalent:@""];

        //[data.menuItem setSubmenu:data.menu];
    }
    menuData[this] = data;
}

AppMenu& AppMenu::AddChild(const std::string &text, const std::string& shortcut) {
    children.push_back( new AppMenu(this, text, shortcut) );
    return *children.back();
}

