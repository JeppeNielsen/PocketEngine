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

AppMenu& AppMenu::AddChild(const std::string &text, const std::string& shortcut) {
    children.push_back( new AppMenu() );
    return *children.back();
}

void AppMenu::ShowPopup(const Vector2& location) {

}

void AppMenu::InitializeMainMenu() {

}

void AppMenu::InitializePopUp() {

}

void AppMenu::CreateAppMenu(AppMenu* parent, const std::string& text, const std::string& shortcut) {


}
