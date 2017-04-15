//
//  AutocompleterTextEditorSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 09/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "AutocompleterTextEditorSystem.hpp"
#include "Gui.hpp"
#include "VirtualTreeList.hpp"
#include "VirtualTreeListSpawner.hpp"
#include "AutocompleterEntry.hpp"

using namespace Pocket;

void AutocompleterTextEditorSystem::Initialize() {
    activeTextEditor = 0;
    root->Input().ButtonDown.Bind(this, &AutocompleterTextEditorSystem::ButtonDown);
    gui = root->CreateSystem<Gui>();
    listBox = 0;
}

void AutocompleterTextEditorSystem::Destroy() {
    root->Input().ButtonDown.Unbind(this, &AutocompleterTextEditorSystem::ButtonDown);
}

void AutocompleterTextEditorSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<TextBox>()->Active.Changed.Bind(this, &AutocompleterTextEditorSystem::TextBoxActivated, object);
}

void AutocompleterTextEditorSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<TextBox>()->Active.Changed.Bind(this, &AutocompleterTextEditorSystem::TextBoxActivated, object);
    if (activeTextEditor == object) {
        activeTextEditor = 0;
    }
}

void AutocompleterTextEditorSystem::TextBoxActivated(Pocket::GameObject *object) {
    if (object->GetComponent<TextBox>()->Active) {
        activeTextEditor = object;
    } else {
        if (object == activeTextEditor) {
            activeTextEditor = 0;
        }
    }
}

void AutocompleterTextEditorSystem::ButtonDown(ButtonEvent e) {
    if (!activeTextEditor) return;
    TextEditor* textEditor = activeTextEditor->GetComponent<TextEditor>();
    
    if (e.Id == "p") {
        Autocompleter* autoCompleter = activeTextEditor->GetComponent<Autocompleter>();
        Point cursor = textEditor->CursorToCartesian(textEditor->Cursor);
        autoCompleter->DoAutoComplete(cursor.y+1, cursor.x);
        
        if (listBox) {
            listBox->Remove();
        }
        
        GameObject* pivot;
        listBox = gui->CreateListbox(root, "Box", 0, {400,200}, &pivot);
        
        
        
        VirtualTreeList* treeView = pivot->AddComponent<VirtualTreeList>();
        treeView->Root = activeTextEditor->GetComponentOwner<Autocompleter>();
        
        treeView->Root()->ToJson(std::cout);
        
        treeView->SetNodeExpanded(treeView->Root(), true);
        treeView->Pivot = listBox;
        
        auto spawner = pivot->AddComponent<VirtualTreeListSpawner>();
        
        spawner->OnCreate = [&, this](auto& n) {
            
            AutocompleterEntry* entry = n.node->template GetComponent<AutocompleterEntry>();
            
            GameObject* elementBackground = gui->CreateControl(n.parent, "TextBox", 0, {200,n.height});
            elementBackground->AddComponent<Colorable>()->Color = Colour::White();
            elementBackground->GetComponent<Touchable>()->ClickThrough = true;
            
            GameObject* label = gui->CreateLabel(elementBackground, {0,0}, {200,n.height}, 0, entry->text, 12);
            
            label->GetComponent<Label>()->HAlignment = Font::HAlignment::Left;
            label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
            label->AddComponent<Colorable>()->Color = Colour::Black();
        };
        
        spawner->OnRemove = [this] (auto& n) {};
    }
}
