//
//  TextEditorEditSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 29/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "TextEditorEditSystem.hpp"


void TextEditorEditSystem::Initialize() {
    activeTextEditor = 0;
    root->Input().ButtonDown.Bind(this, &TextEditorEditSystem::ButtonDown);
}

void TextEditorEditSystem::Destroy() {
    root->Input().ButtonDown.Unbind(this, &TextEditorEditSystem::ButtonDown);
}

void TextEditorEditSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<TextBox>()->Active.Changed.Bind(this, &TextEditorEditSystem::TextBoxActivated, object);
    object->GetComponent<TextBox>()->DeselectOnEnter = false;
}

void TextEditorEditSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<TextBox>()->Active.Changed.Bind(this, &TextEditorEditSystem::TextBoxActivated, object);
    if (activeTextEditor == object) {
        activeTextEditor = 0;
    }
}

void TextEditorEditSystem::TextBoxActivated(Pocket::GameObject *object) {
    if (object->GetComponent<TextBox>()->Active) {
        activeTextEditor = object;
    } else {
        if (object == activeTextEditor) {
            activeTextEditor = 0;
        }
    }
}

void TextEditorEditSystem::ButtonDown(std::string button) {
    if (!activeTextEditor) return;
    TextEditor* textEditor = activeTextEditor->GetComponent<TextEditor>();
    
    std::string t1 = textEditor->text.substr(0, textEditor->Cursor);
    std::string t2 = textEditor->text.substr(textEditor->Cursor, textEditor->text.size() - textEditor->Cursor());
    
    if (button == "\r") {
        button = "\n";
    } else if (button == "\t") {
        button = "   ";
        textEditor->Cursor+=2;
    } else if (button == "\x7f") {
        t1 = t1.substr(0, t1.size()-1);
        button = "";
        textEditor->Cursor -= 2;
    } else if (button == "") {
        textEditor->MoveCursor({0,1});
        return;
    } else if (button == "") {
        textEditor->MoveCursor({0,-1});
        return;
    } else if (button == "") {
        textEditor->MoveCursor({-1,0});
        return;
    } else if (button == "") {
        textEditor->MoveCursor({1,0});
        return;
    } else if (button.size()>1) {
        return;
    }
    
    textEditor->text = t1 + button + t2;
    textEditor->Lines.MakeDirty();
    
    textEditor->Cursor++;
    
    if (textEditor->Cursor<0) {
        textEditor->Cursor = 0;
    }
    if (textEditor->Cursor>=textEditor->text.size()) {
        textEditor->Cursor = (int)textEditor->text.size() - 1;
    }
    if (textEditor->Cursor<0) {
        textEditor->Cursor = 0;
    }
}
