//
//  TextEditorScrollSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 06/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "TextEditorScrollSystem.hpp"

void TextEditorScrollSystem::Initialize() {
    activeTextEditor = 0;
    scrollMovedFromHere = false;
    root->Input().ScrollChanged.Bind(this, &TextEditorScrollSystem::InputScrollChanged);
}

void TextEditorScrollSystem::Destroy() {
    root->Input().ScrollChanged.Unbind(this, &TextEditorScrollSystem::InputScrollChanged);
}

void TextEditorScrollSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<TextBox>()->Active.Changed.Bind(this, &TextEditorScrollSystem::TextBoxActivated, object);
    TextBoxActivated(object);
}

void TextEditorScrollSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<TextBox>()->Active.Changed.Bind(this, &TextEditorScrollSystem::TextBoxActivated, object);
     if (object == activeTextEditor) {
        activeTextEditor = 0;
        TextEditorRenderer* renderer = object->GetComponent<TextEditorRenderer>();
        renderer->Offset.Changed.Unbind(this, &TextEditorScrollSystem::OffsetChanged, object);
    }
}

void TextEditorScrollSystem::TextBoxActivated(Pocket::GameObject *object) {
    if (object->GetComponent<TextBox>()->Active) {
        activeTextEditor = object;
        TextEditorRenderer* renderer = object->GetComponent<TextEditorRenderer>();
        renderer->Offset.Changed.Bind(this, &TextEditorScrollSystem::OffsetChanged, object);
    } else {
        if (object == activeTextEditor) {
            activeTextEditor = 0;
            TextEditorRenderer* renderer = object->GetComponent<TextEditorRenderer>();
            renderer->Offset.Changed.Unbind(this, &TextEditorScrollSystem::OffsetChanged, object);
        }
    }
}

void TextEditorScrollSystem::OffsetChanged(GameObject* object) {
    if (scrollMovedFromHere) return;
    isScrolling = false;
}

void TextEditorScrollSystem::Update(float dt) {
    if (!activeTextEditor) return;
    if (!isScrolling) return;
    
    TextEditorRenderer* renderer = activeTextEditor->GetComponent<TextEditorRenderer>();

    Vector2 targetVector = scrollTarget - renderer->Offset;

    scrollMovedFromHere = true;
    renderer->Offset += targetVector * 20.0f * dt;
    scrollMovedFromHere = true;
    
    if (targetVector.Length()<1.0f) {
        isScrolling = false;
    }
}

void TextEditorScrollSystem::InputScrollChanged(float scrollAmount) {
    if (!activeTextEditor) return;
    isScrolling = true;
    scrollTarget = activeTextEditor->GetComponent<TextEditorRenderer>()->Offset + Vector2(0, -scrollAmount*20.0f);
}
