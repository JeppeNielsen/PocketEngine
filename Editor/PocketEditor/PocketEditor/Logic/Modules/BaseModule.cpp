//
//  BaseModule.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 28/12/2016.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "BaseModule.hpp"

BaseModule::~BaseModule() {}

void BaseModule::Initialize(Pocket::EngineContext *engineContext, EditorContext *context) {
    this->engineContext = engineContext;
    this->context = context;
    OnInitialize();
}

void BaseModule::Create() {
    OnCreate();
}

void BaseModule::OnCreate() {}
void BaseModule::OnInitialize() {}
