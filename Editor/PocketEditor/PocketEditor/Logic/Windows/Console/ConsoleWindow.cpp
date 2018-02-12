//
//  ConsoleWindow.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 25/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "ConsoleWindow.hpp"
#include "VirtualTreeList.hpp"
#include "VirtualTreeListSpawner.hpp"

std::string ConsoleWindow::Name() { return "Console"; }

void ConsoleWindow::OnInitialize() {

}

Vector2 ConsoleWindow::Size() {
    return {500, 200 };
}

void ConsoleWindow::OnCreate() {
    Gui& gui = context->Gui();

    
    GameObject* pivot;
    listBox = gui.CreateListbox(window, "Box", {0,0}, {500,window->GetComponent<Sizeable>()->Size().y-50}, &pivot);
    listBox->RemoveComponent<Sprite>();
    
    pivot->GetComponent<Transform>()->Position = {0,400};
    
    treeView = pivot->AddComponent<VirtualTreeList>();
    treeView->Root = context->Log().LogRoot();
    treeView->SetNodeExpanded(context->Log().LogRoot(), true);
    treeView->Pivot = listBox;
    treeView->ExpandedHashFunction = [this] (GameObject* go) {
        return treeView->DefaultExpandedHashFunction(go);
    };
    
    auto spawner = pivot->AddComponent<VirtualTreeListSpawner>();
    
    spawner->OnCreate = [&, this](auto& n) {
        LogMessage* message = n.node->template GetComponent<LogMessage>();
        
        GameObject* clone = gui.CreateControl(n.parent, "Box", 0, {500,n.height});
        clone->RemoveComponent<Touchable>();
        gui.CreateControl(clone, "TextBox", 0, {25,25});
        
        GameObject* selectButton = gui.CreateControl(clone, "TextBox", {25,0}, {500-25,25});
        
        auto l = gui.CreateLabel(selectButton, {0,0}, {500-25,25}, 0, message ? message->message : "", 12);
        l->GetComponent<Colorable>()->Color = Colour::Black();
        l->GetComponent<Label>()->HAlignment = Font::HAlignment::Left;
        l->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
    
        selectButton->GetComponent<Touchable>()->Click.Bind(this, &ConsoleWindow::Clicked, n.node);
    };
    
    spawner->OnRemove = [] (auto n) {
        
    };
    
    
    compilingText = gui.CreateLabel(window, 0, Size(), 0, "Compiling!!!", 20);
    compilingText->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
    compilingText->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
    
    compilingText->Hierarchy().Enabled = false;
    
    context->Project().IsCompiling.Changed.Bind([this] {
        compilingText->Hierarchy().Enabled = context->Project().IsCompiling;
    });
}

void ConsoleWindow::Clicked(Pocket::TouchData d, Pocket::GameObject *node) {
    LogMessage* message = node->GetComponent<LogMessage>();
    
    std::cout << "Console item clicked : " << (message ? message->message : "root") << std::endl;
}

