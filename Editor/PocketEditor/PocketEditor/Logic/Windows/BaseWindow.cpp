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
    this->context->Project().Worlds.ActiveWorld.Changed.Bind(this, &BaseWindow::ActiveWorldChangedInternal);
    OnInitialize();
}



void BaseWindow::Create() {

    Gui& gui = context->Gui();
    
    window = gui.CreateControl(0, "Box", {200,400});
    GameObject* bar = gui.CreateControl(window, "TextBox", {200,50});
    bar->GetComponent<Transform>()->Position = {0,400-50};
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

GameObject* BaseWindow::CreateButton(GameObject* parent, const Vector2& position, const Vector2& size, const std::string& text) {
    Gui& gui = context->Gui();
    
    GameObject* button = gui.CreateControl(parent, "Box", position, size);
    GameObject* label = gui.CreateLabel(button, 0, size, 0, text, 20);
    label->GetComponent<Label>()->HAlignment = Font::Center;
    label->GetComponent<Label>()->VAlignment = Font::Middle;
    label->GetComponent<Colorable>()->Color = Colour::Black();

    return button;
}

void BaseWindow::ActiveWorldChangedInternal() {
    OpenWorld* prev = context->Project().Worlds.ActiveWorld.PreviousValue();
    OpenWorld* current = context->Project().Worlds.ActiveWorld;
    ActiveWorldChanged(prev, current);
}

void BaseWindow::ActiveWorldChanged(OpenWorld *old, OpenWorld *current) {
}
