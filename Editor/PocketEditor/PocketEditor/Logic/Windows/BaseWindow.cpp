//
//  BaseWindow.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "BaseWindow.hpp"

BaseWindow::~BaseWindow() { }

void BaseWindow::Initialize(EditorContext *context) {
    this->context = context;
    OnInitialize();
}

void BaseWindow::Create() {

    Gui& gui = context->Gui();
    
    window = gui.CreateControl(0, "Box", {200,800});
    GameObject* bar = gui.CreateControl(window, "TextBox", {200,50});
    bar->GetComponent<Transform>()->Position = {0,800-50};
    bar->RemoveComponent<Touchable>();
    bar->AddComponent<Touchable>(window);
    window->AddComponent<Draggable>();
    GameObject* label = gui.CreateLabel(bar, 0, {200,50}, 0, Name(), 20);
    label->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
    label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
    label->GetComponent<Colorable>()->Color = Colour::Black();
    
    OnCreate();
}

void BaseWindow::OnInitialize() {
    
}

void BaseWindow::OnCreate() {

}
