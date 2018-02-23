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
#include <algorithm>
#include "StringHelper.hpp"

using namespace Pocket;

void AutocompleterTextEditorSystem::Initialize() {
    activeTextEditor = 0;
    gui = root->CreateSystem<Gui>();
    listBox = 0;
}

void AutocompleterTextEditorSystem::Destroy() {
}

void AutocompleterTextEditorSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<TextBox>()->Active.Changed.Bind(this, &AutocompleterTextEditorSystem::TextBoxActivated, object);
    
    object->GetComponent<TextEditor>()->AutoCompleteDisabled.Bind(this, &AutocompleterTextEditorSystem::AutoCompleteDisabled, object);
    object->GetComponent<Autocompleter>()->OnAutoComplete.Bind(this, &AutocompleterTextEditorSystem::OnAutoComplete, object);
    object->GetComponent<TextEditor>()->AutoCompleteEvent.Bind(this, &AutocompleterTextEditorSystem::OnAutoCompleteEvent, object);
    object->GetComponent<TextEditor>()->AutoCompleteEnabled.Bind(this, &AutocompleterTextEditorSystem::AutoCompleteDisabled, object);
}

void AutocompleterTextEditorSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<TextBox>()->Active.Changed.Unbind(this, &AutocompleterTextEditorSystem::TextBoxActivated, object);
    object->GetComponent<TextEditor>()->AutoCompleteDisabled.Unbind(this, &AutocompleterTextEditorSystem::AutoCompleteDisabled, object);
    object->GetComponent<Autocompleter>()->OnAutoComplete.Unbind(this, &AutocompleterTextEditorSystem::OnAutoComplete, object);
    object->GetComponent<TextEditor>()->AutoCompleteEvent.Unbind(this, &AutocompleterTextEditorSystem::OnAutoCompleteEvent, object);
    object->GetComponent<TextEditor>()->AutoCompleteEnabled.Unbind(this, &AutocompleterTextEditorSystem::AutoCompleteDisabled, object);
    
    
    if (activeTextEditor == object) {
        activeTextEditor = 0;
        RemoveListBox();
    }
}

void AutocompleterTextEditorSystem::TextBoxActivated(Pocket::GameObject *object) {
    if (object->GetComponent<TextBox>()->Active) {
        activeTextEditor = object;
    } else {
        if (object == activeTextEditor) {
            RemoveListBox();
        }
    }
}

void AutocompleterTextEditorSystem::AutoCompleteDisabled(Pocket::GameObject *object) {
    RemoveListBox();
}

void AutocompleterTextEditorSystem::OnAutoComplete(std::vector<Autocompleter::Result> results, Pocket::GameObject *object) {
    RemoveListBox();
    if (!activeTextEditor) return;
    
    TextEditor* textEditor = object->GetComponent<TextEditor>();
    if (!textEditor->AutoCompleteActive()) return;
    
    TextEditorRenderer* renderer = activeTextEditor->GetComponent<TextEditorRenderer>();
    Font* font = activeTextEditor->GetComponent<Font>();
    Vector2 size = activeTextEditor->GetComponent<Sizeable>()->Size;
    
    //Autocompleter* autoCompleter = activeTextEditor->GetComponent<Autocompleter>();
    Point cursor = textEditor->CursorToCartesian(textEditor->AutoCompleteCursor);
    //auto results = autoCompleter->DoAutoComplete(textEditor->text, cursor.y+1, cursor.x+1);
    
    
    
    if (results.empty()) return;
    
    GameObject* autocompleterRoot = activeTextEditor->GetComponentOwner<Autocompleter>();
    
    float fontSize = renderer->fontSize;
    float spacing = font->GetSpacing(fontSize);
    
    float uiFontSize = 14;
    float uiFontSpacing = gui->GetFonts()[0]->GetComponent<Font>()->GetSpacing(uiFontSize);

    Vector2 scaler = { spacing, renderer->fontSize };
    
    Vector2 maxSize = 0;
    for(auto& r : results) {
        maxSize.x = MathHelper::Max(maxSize.x, r.GetDisplayedText().size()*uiFontSpacing);
    }
    maxSize.y = MathHelper::Min(200.0f, 14.0f * results.size());

    Vector3 position = {
        -renderer->Offset().x + cursor.x * scaler.x,
        renderer->Offset().y + size.y - fontSize - cursor.y * scaler.y
    };

    GameObject* pivot;
    listBox = gui->CreateListbox(activeTextEditor, "Box", position-Vector3(0,maxSize.y), maxSize, &pivot);
    pivot->GetComponent<Transform>()->Position = {0,maxSize.y};
    
    treeView = pivot->AddComponent<VirtualTreeList>();
    treeView->ItemHeight = 14;
    treeView->Root = autocompleterRoot;
    
    //treeView->Root()->ToJson(std::cout);
    
    treeView->SetNodeExpanded(treeView->Root(), true);
    treeView->Pivot = listBox;
    
    treeView->PredicateFunction = [ textEditor, this ] (GameObject* object) {
        std::string autocompleteText = textEditor->GetAutoCompleteString();
        AutocompleterEntry* entry = object->GetComponent<AutocompleterEntry>();
        //std::cout << autocompleteText << std::endl;
        return StringHelper::StartsWith(entry->entry.GetText(), autocompleteText);// entry->text.find(autocompleteText) != std::string::npos;
    };
    
    auto spawner = pivot->AddComponent<VirtualTreeListSpawner>();
    
    spawner->OnCreate = [=](auto& n) {
        
        AutocompleterEntry* entry = n.node->template GetComponent<AutocompleterEntry>();
        
        GameObject* elementBackground = gui->CreateControl(n.parent, "Box", 0, {maxSize.x,n.height});
        elementBackground->AddComponent<Colorable>()->Color = Colour::White();
        elementBackground->GetComponent<Touchable>()->ClickThrough = true;
        
        GameObject* label = gui->CreateLabel(elementBackground, {0,0}, {maxSize.x,n.height}, 0, entry->entry.GetDisplayedText(), uiFontSize);
        
        label->GetComponent<Label>()->HAlignment = Font::HAlignment::Left;
        label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
        label->AddComponent<Colorable>()->Color = Colour::Black();
        
        elementBackground->GetComponent<Touchable>()->Click.Bind([entry, this] (TouchData d) {
            TextEditor* textEditor = activeTextEditor->GetComponent<TextEditor>();
            textEditor->InsertText(textEditor->Cursor, entry->entry.GetText());
            RemoveListBox();
        });
    };
    
    spawner->OnRemove = [this] (auto& n) {};
    
    for(auto& r : results) {
        if (r.cursorKind == 700) {
            
            int currentParemeterStart = -1;
            int currentParemeterEnd = -1;
            
            
            std::string text = "";
            for(auto& c : r.chunks) {
                if (c.kind == 5) { // current parameter
                    currentParemeterStart = (int)text.size();
                    currentParemeterEnd = currentParemeterStart + (int)c.text.size();
                }
                
                text += c.text;
    
                if (c.kind == 15 || //result type
                    c.kind == 14 // comma
                    ) {
                   text+=" ";
                }
            }
            
            
            
            
            //float width = gui->GetFonts()[0]->GetComponent<Font>()->GetSpacing(20)*text.size();
            
            functionParametersListBox = gui->CreateControl(object, "TextBox", position + Vector2(0,20), {uiFontSpacing * text.size()+20,20});
                GameObject* label = gui->CreateLabel(functionParametersListBox, {10,0}, functionParametersListBox->GetComponent<Sizeable>()->Size, 0, text, uiFontSize);
            label->GetComponent<Label>()->HAlignment = Font::HAlignment::Left;
            label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
            label->AddComponent<Colorable>()->Color = Colour::Black();
            
            if (currentParemeterStart>=0) {
                GameObject* param = gui->CreateControl(functionParametersListBox, "Box", { 10 + currentParemeterStart * uiFontSpacing, 0} , { (currentParemeterEnd - currentParemeterStart) * uiFontSpacing, 20});
                param->AddComponent<Colorable>()->Color = Colour::Green(0.5f);
            
            }
            
            break;
        }
    }
}

void AutocompleterTextEditorSystem::RemoveListBox() {
    if (listBox) {
        listBox->Remove();
        listBox = 0;
    }
    if (functionParametersListBox) {
        functionParametersListBox->Remove();
        functionParametersListBox = 0;
    }
}

AutocompleterEntry* AutocompleterTextEditorSystem::FindAutocompleterEntryByIndex(int index) {
    if (!treeView) return 0;
    TextEditor* textEditor = activeTextEditor->GetComponent<TextEditor>();
    std::string autocompleteText = textEditor->GetAutoCompleteString();
    
    int i = 0;
    for(auto child : treeView->Root()->Hierarchy().Children()) {
        AutocompleterEntry* entry = child->GetComponent<AutocompleterEntry>();
        bool found = StringHelper::StartsWith(entry->entry.GetText(), autocompleteText); //entry->text.find(autocompleteText) != std::string::npos;
        if (!found) continue;
        if (i == index) return entry;
        i++;
    }
    return 0;
}

void AutocompleterTextEditorSystem::OnAutoCompleteEvent(TextEditor::AutoCompleteEventType eventType, Pocket::GameObject *object) {
    if (!activeTextEditor) return;
    if (!listBox) return;
    
    if (eventType == TextEditor::AutoCompleteEventType::TokenSelected) {
    
        auto entry = FindAutocompleterEntryByIndex(selectedTokenIndex);
        if (entry) {
            TextEditor* textEditor = activeTextEditor->GetComponent<TextEditor>();
            textEditor->EraseText(textEditor->AutoCompleteCursor, textEditor->Cursor);
            textEditor->InsertText(textEditor->AutoCompleteCursor, entry->entry.GetText());
            textEditor->Cursor = textEditor->AutoCompleteCursor + (int)entry->entry.GetText().size();
        }
        RemoveListBox();
    }
}
