//
//  AppMenu.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 21/07/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "AppMenu.hpp"

using namespace Pocket;

AppMenu::AppMenu() { }
AppMenu::~AppMenu() { }
AppMenu::AppMenu(const std::string& text) : text(text) {}

AppMenu& AppMenu::AddChild(const std::string &text) {
    children.push_back( AppMenu(text) );
    return children.back();
}

