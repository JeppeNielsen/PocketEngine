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

AppMenu& AppMenu::AddChild(const std::string& text, const std::string& shortcut) {
    return *this;
}

void AppMenu::ShowPopup(const Vector2& location) {
    
}

void AppMenu::CreateAppMenu(AppMenu* parent, const std::string& text, const std::string& shortcut) {

}

void AppMenu::InitializeMainMenu() {}
void AppMenu::InitializePopUp() {}
