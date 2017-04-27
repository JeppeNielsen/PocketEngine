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
    currentWorld = 0;
    this->context = context;
    this->context->Project().Worlds.ActiveWorld.Changed.Bind(this, &BaseWindow::ActiveWorldChangedInternal);
    this->context->Project().IsCompiling.Changed.Bind([this] () {
        PostCompile();
    });
    OnInitialize();
}



void BaseWindow::Create() {

    Gui& gui = context->Gui();
    
    Vector2 size = Size();
    
    window = gui.CreateControl(0, "Box", size);
    
    if (CreateBar()) {
        GameObject* bar = gui.CreateControl(window, "TextBox", {size.x,50});
        bar->GetComponent<Transform>()->Position = {0,size.y-50};
        bar->RemoveComponent<Touchable>();
        bar->AddComponent<Touchable>(window);
        window->AddComponent<Draggable>();
        GameObject* label = gui.CreateLabel(bar, 0, {size.x,50}, 0, Name(), 20);
        label->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
        label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
        label->GetComponent<Colorable>()->Color = Colour::Black();
    }
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
    label->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
    label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
    label->GetComponent<Colorable>()->Color = Colour::Black();

    return button;
}

void BaseWindow::ActiveWorldChangedInternal() {
    OpenWorld* prev = context->Project().Worlds.ActiveWorld.PreviousValue();
    OpenWorld* current = context->Project().Worlds.ActiveWorld;
    currentWorld = current;
    ActiveWorldChanged(prev, current);
}

void BaseWindow::ActiveWorldChanged(OpenWorld *old, OpenWorld *current) {
}

bool BaseWindow::CreateBar() {
    return true;
}

void BaseWindow::PostCompile() {

}

Vector2 BaseWindow::Size() {
    return {200,400};
}
