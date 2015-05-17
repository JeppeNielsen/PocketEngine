//
//  MenuButtonSystem.cpp
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/27/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "MenuButtonSystem.hpp"

void MenuButtonSystem::Initialize() {
    AddComponent<MenuButton>();
    AddComponent<Touchable>();
}

void MenuButtonSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Click += event_handler(this, &MenuButtonSystem::ButtonClick, object);
}

void MenuButtonSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Click -= event_handler(this, &MenuButtonSystem::ButtonClick, object);
}

void MenuButtonSystem::ButtonClick(Pocket::TouchData d, Pocket::GameObject *object) {
    MenuButton* menuButton = object->GetComponent<MenuButton>();
    if (!menuButton->menu) return;
    menuButton->menu->ActiveMenu = menuButton->menuToActivate;
}
