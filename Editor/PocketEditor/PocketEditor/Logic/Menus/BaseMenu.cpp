//
//  BaseMenu.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 03/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "BaseMenu.hpp"


BaseMenu::~BaseMenu() {}

void BaseMenu::Initialize(Pocket::EngineContext *engineContext, EditorContext *context) {
    this->engineContext = engineContext;
    this->context = context;
    OnInitialize();
}

void BaseMenu::Create() {
    menu = &engineContext->Menu().AddChild(Name());
    OnCreate();
}

void BaseMenu::OnCreate() {}
void BaseMenu::OnInitialize() {}
