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

void TextEditorEditSystem::ButtonDown(ButtonEvent e) {
    if (!activeTextEditor) return;
    TextEditor* textEditor = activeTextEditor->GetComponent<TextEditor>();
    bool shift = e.modifierKey == ModifierKey::Shift;
    bool wasSelectionActive = textEditor->SelectionActive;
    if (!shift && textEditor->SelectionActive) {
        textEditor->SelectionActive = false;
    }
    
    std::string button = e.Id;
    
    if (button == "") {
        if (textEditor->AutoCompleteActive) {
            textEditor->AutoCompleteEvent(TextEditor::AutoCompleteEventType::PrevToken);
            return;
        }
        if (shift) {
            textEditor->MoveSelection({0,1});
        }
        textEditor->MoveCursor({0,1});
        return;
    } else if (button == "") {
        if (textEditor->AutoCompleteActive) {
            textEditor->AutoCompleteEvent(TextEditor::AutoCompleteEventType::NextToken);
            return;
        }
        if (shift) {
            textEditor->MoveSelection({0,-1});
        }
        textEditor->MoveCursor({0,-1});
        return;
    } else if (button == "") {
        
        if (shift) {
            textEditor->MoveSelection({-1,0});
        }
        textEditor->MoveCursor({-1,0});
        textEditor->DisableAutoComplete();
        return;
    } else if (button == "") {
        if (shift) {
            textEditor->MoveSelection({1,0});
        }
        textEditor->MoveCursor({1,0});
        textEditor->DisableAutoComplete();
        return;
    } else if (button.size()>1) {
        textEditor->DisableAutoComplete();
        return;
    }
    
    
    if (wasSelectionActive) {
        
        int min;
        int max;
        if (textEditor->Selection().x<textEditor->Selection().y) {
            min = textEditor->Selection().x;
            max = textEditor->Selection().y;
        } else {
            min = textEditor->Selection().y;
            max = textEditor->Selection().x;
        }
        
        if (min!=max) {
            textEditor->text.erase(textEditor->text.begin() + min, textEditor->text.begin() + max - 1);
            textEditor->Cursor = min + 1;
        }
        
        textEditor->SelectionActive = false;
    }
    
    std::string t1 = textEditor->text.substr(0, textEditor->Cursor);
    std::string t2 = textEditor->text.substr(textEditor->Cursor, textEditor->text.size() - textEditor->Cursor());
    
    
    bool returnClicked = button == "\r";
    bool tabClicked = button == "\t";
    bool backspaceClicked = button == "\x7f";
    bool escapeClicked = button == "\e";
    
    if (returnClicked) {
        if (textEditor->AutoCompleteActive) {
            textEditor->AutoCompleteEvent(TextEditor::AutoCompleteEventType::TokenSelected);
            textEditor->DisableAutoComplete();
            return;
        }
        button = "\n";
    } else if (tabClicked) {
        if (textEditor->AutoCompleteActive) {
            textEditor->AutoCompleteEvent(TextEditor::AutoCompleteEventType::TokenSelected);
            return;
        }
        button = "   ";
        textEditor->Cursor+=2;
    } else if (backspaceClicked) {
        t1 = t1.substr(0, t1.size()-1);
        textEditor->TextInserted({(int)t1.size(), -1});
        button = "";
        textEditor->Cursor -= 2;
    } else if (escapeClicked) {
        button = "";
        textEditor->DisableAutoComplete();
        return;
    }
    
    bool wasCursorAtWhiteSpace = textEditor->IsCursorAtWhiteSpace();
    
    textEditor->text = t1 + button + t2;
    textEditor->TextInserted({(int)t1.size(), (int)button.size()});
    
    textEditor->Cursor++;
    textEditor->Lines.MakeDirty();
    
    if (textEditor->Cursor()<textEditor->AutoCompleteCursor) {
        textEditor->DisableAutoComplete();
    }
    
    if ((backspaceClicked && textEditor->IsCursorAtWhiteSpace()) || button == " " || button == ";") {
        textEditor->DisableAutoComplete();
    } else if (returnClicked || tabClicked || escapeClicked) {
        textEditor->DisableAutoComplete();
    } else if (wasCursorAtWhiteSpace) {
        textEditor->EnableAutoComplete(textEditor->Cursor - 1);
    } else if (textEditor->IsCursorAtToken(".") ||
        textEditor->IsCursorAtToken("->") ||
        textEditor->IsCursorAtToken("::") ||
        textEditor->IsCursorAtToken("(") ||
        textEditor->IsCursorAtToken(",") ||
        textEditor->IsCursorAtToken("<")) {
        textEditor->EnableAutoComplete(textEditor->Cursor);
    }
    
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
