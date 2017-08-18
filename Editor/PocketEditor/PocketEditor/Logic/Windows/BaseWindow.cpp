//
//  BaseWindow.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "BaseWindow.hpp"

BaseWindow::~BaseWindow() { }

GameObject* BaseWindow::Window() { return window; }

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
    
    GameObject* windowObject = gui.CreateControl(0, "Box", size);
    
    if (CreateBar()) {
    
        /*
        GameObject* bar = gui.CreateControl(window, "TextBox", {size.x,50});
        bar->GetComponent<Transform>()->Position = {0,size.y-50};
        bar->RemoveComponent<Touchable>();
        bar->AddComponent<Touchable>(window);
        window->AddComponent<Draggable>();
        GameObject* label = gui.CreateLabel(bar, 0, {size.x,50}, 0, Name(), 20);
        label->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
        label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
        label->GetComponent<Colorable>()->Color = Colour::Black();
        */
        
        windowObject->AddComponent<Draggable>();
        windowObject->AddComponent<Panel>();
        windowObject->AddComponent<Layouter>()->ChildrenLayoutMode = Layouter::LayoutMode::Vertical;
        
        GameObject* content = gui.CreateLayout(windowObject, {200,200}, {2000,2000}, {2000,2000}, Layouter::LayoutMode::Vertical);
        
//        GameObject* horizontal = gui.CreateLayout(content, 20, {2000,20}, {2000,20}, Layouter::LayoutMode::Horizontal);
//        {
//            gui.CreateLayout(horizontal, 20, 20, {50,20});
//            GameObject* button = gui.CreateTextBox(horizontal, "Box", 0, 0, 0, "Text", 20);//(horizontal, "Box", 30, {200,30}, {2000,30});
//            gui.AddLayouter(button, 30, {200,30}, {2000,30});
//            gui.CreateLayout(horizontal, 20, 20, {50,20});
//        }
        
        
        GameObject* barArea = gui.CreateLayoutControl(windowObject, "Box", 20, {2000,20}, {2000,20}, Layouter::LayoutMode::Horizontal);
        barArea->RemoveComponent<Renderable>();
        barArea->RemoveComponent<Touchable>();
        
        GameObject* bar = gui.CreateLayoutControl(barArea, "TextBox", 20, {2000,20}, {2000,20});
        bar->AddComponent<Droppable>()->OnCreate = [this](GameObject* o, TouchData d) -> GameObject* {
            Vector3 position = o->GetComponent<Transform>()->World().TransformPosition(0);
            GameObject* control = context->Gui().CreateControl(0, "Box", position, {200,25});
            return control;
        };
        gui.AddLayouter(bar, 20, {130,20}, {130,20});
        bar->AddComponent<PanelDropper>()->panel = windowObject;
        auto label = gui.CreateLabel(bar, 0, 0, 0, Name(), 16);
        label->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
        label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
        label->GetComponent<Colorable>()->Color = Colour::Black();
        label->ReplaceComponent<Sizeable>(bar);
        
        
        

        
        
        
        
        window = content;
    } else {
        window = windowObject;
    }
    OnCreate();

//    if (CreateBar()) {
//        GameObject* fill = gui.CreateLayout(window, 20, {2000,2000}, {2000,2000});
//    }
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
