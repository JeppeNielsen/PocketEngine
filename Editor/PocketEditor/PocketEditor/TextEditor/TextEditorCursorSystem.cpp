//
//  TextEditorCursorSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 29/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "TextEditorCursorSystem.hpp"
#include "Transform.hpp"
#include "Renderable.hpp"
#include "Mesh.hpp"
#include "Orderable.hpp"

void TextEditorCursorSystem::Initialize() {
    cursorObject = 0;
    activeTextEditor = 0;
}

void TextEditorCursorSystem::Destroy() {
}

void TextEditorCursorSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<TextBox>()->Active.Changed.Bind(this, &TextEditorCursorSystem::TextBoxActivated, object);
    TextBoxActivated(object);
}

void TextEditorCursorSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<TextBox>()->Active.Changed.Bind(this, &TextEditorCursorSystem::TextBoxActivated, object);
     if (object == activeTextEditor) {
        activeTextEditor = 0;
        cursorObject->Remove();
        TextEditor* textEditor = object->GetComponent<TextEditor>();
        textEditor->Cursor.Changed.Unbind(this, &TextEditorCursorSystem::CursorChanged, object);
        
        TextEditorRenderer* renderer = object->GetComponent<TextEditorRenderer>();
        renderer->Offset.Changed.Unbind(this, &TextEditorCursorSystem::OffsetChanged, object);
    }
}

void TextEditorCursorSystem::TextBoxActivated(Pocket::GameObject *object) {
    TextEditor* textEditor = object->GetComponent<TextEditor>();
    
    if (object->GetComponent<TextBox>()->Active) {
        activeTextEditor = object;
        
        TextEditorRenderer* renderer = object->GetComponent<TextEditorRenderer>();
    
        Vector2 size = { renderer->fontSize*0.2f, renderer->fontSize };
        
        cursorObject = object->CreateChild();
        cursorObject->AddComponent<Transform>();
        cursorObject->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(size*0.5f, size, Colour::Black());
        cursorObject->AddComponent<Renderable>();
        cursorObject->AddComponent<Orderable>();
        
        textEditor->Cursor.Changed.Bind(this, &TextEditorCursorSystem::CursorChanged, object);
        renderer->Offset.Changed.Bind(this, &TextEditorCursorSystem::OffsetChanged, object);
        
        CursorChanged(object);
        
    } else {
        if (object == activeTextEditor) {
            activeTextEditor = 0;
            cursorObject->Remove();
            textEditor->Cursor.Changed.Unbind(this, &TextEditorCursorSystem::CursorChanged, object);
            
            TextEditorRenderer* renderer = object->GetComponent<TextEditorRenderer>();
            renderer->Offset.Changed.Unbind(this, &TextEditorCursorSystem::OffsetChanged, object);
        }
    }
}

void TextEditorCursorSystem::CursorChanged(Pocket::GameObject* object) {
    TextEditor* textEditor = object->GetComponent<TextEditor>();
    TextEditorRenderer* renderer = object->GetComponent<TextEditorRenderer>();
    Font* font = object->GetComponent<Font>();
    Point location = textEditor->CursorToCartesian(textEditor->Cursor);
    
    float fontSize = renderer->fontSize;
    float spacing = font->GetSpacing(fontSize);
    
    Vector2 scaler = { spacing, renderer->fontSize };

    isScrolling = true;
    scrollTarget = {location.x * scaler.x, location.y * scaler.y };
}

void TextEditorCursorSystem::OffsetChanged(GameObject* object) {
    if (scrollMovedFromHere) return;
    isScrolling = false;
}

void TextEditorCursorSystem::UpdateCursorTransform() {
    if (!activeTextEditor) return;
    if (!cursorObject) return;
    
    TextEditor* textEditor = activeTextEditor->GetComponent<TextEditor>();
    TextEditorRenderer* renderer = activeTextEditor->GetComponent<TextEditorRenderer>();
    Font* font = activeTextEditor->GetComponent<Font>();
    Vector2 size = activeTextEditor->GetComponent<Sizeable>()->Size;
    
    Point location = textEditor->CursorToCartesian(textEditor->Cursor);
    
    float fontSize = renderer->fontSize;
    float spacing = font->GetSpacing(fontSize);
    
    Vector2 scaler = { spacing, renderer->fontSize };
    
    cursorObject->GetComponent<Transform>()->Position = {
        -renderer->Offset().x + location.x * scaler.x,
        renderer->Offset().y + size.y - fontSize - location.y * scaler.y
    };
}

void TextEditorCursorSystem::Update(float dt) {
    if (!activeTextEditor) return;
    UpdateCursorTransform();
    if (!isScrolling) return;
    
    TextEditorRenderer* renderer = activeTextEditor->GetComponent<TextEditorRenderer>();
    Vector2 size = activeTextEditor->GetComponent<Sizeable>()->Size;
    
    Vector2 screenPosition = scrollTarget- renderer->Offset;
    Vector2 targetVector = 0;
    
    float margin = renderer->fontSize;
    
    if ((screenPosition.y+renderer->fontSize)<margin) {
        targetVector.y = (screenPosition.y+renderer->fontSize) - margin;
    } else if (screenPosition.y>(size.y-margin)) {
        targetVector.y = screenPosition.y - (size.y-margin);
    }
    
    if (screenPosition.x<0) {
        targetVector.x = screenPosition.x;
    } else if (screenPosition.x>(size.x-margin)) {
        targetVector.x = screenPosition.x - (size.x-margin);
    }

    scrollMovedFromHere = true;
    renderer->Offset += targetVector * 10.0f * dt;
    scrollMovedFromHere = true;
    
    if (targetVector.Length()<1.0f) {
        isScrolling = false;
    }
}
