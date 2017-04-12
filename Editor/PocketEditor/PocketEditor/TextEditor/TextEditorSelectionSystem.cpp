//
//  TextEditorSelectionSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "TextEditorSelectionSystem.hpp"
#include "Camera.hpp"
#include "EngineContext.hpp"
#include <iostream>

void TextEditorSelectionSystem::Initialize() {
    isDown = false;
}

void TextEditorSelectionSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Down.Bind(this, &TextEditorSelectionSystem::Down, object);
    object->GetComponent<Touchable>()->Up.Bind(this, &TextEditorSelectionSystem::Up, object);
}

void TextEditorSelectionSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Down.Unbind(this, &TextEditorSelectionSystem::Down, object);
    object->GetComponent<Touchable>()->Up.Unbind(this, &TextEditorSelectionSystem::Up, object);
    
}

void TextEditorSelectionSystem::Down(Pocket::TouchData e, GameObject* object) {
    if (e.Index!=0) return;
    TextEditor* textEditor = object->GetComponent<TextEditor>();
    textEditor->Cursor = GetCursorPosition(object, e.WorldPosition);
    textEditor->Selection = { textEditor->Cursor, textEditor->Cursor };
    
    isDown = true;
    downObject.object = object;
    downObject.touchData = e;
}


void TextEditorSelectionSystem::Up(Pocket::TouchData e, GameObject* object) {
    if (e.Index!=0) return;
    isDown = false;
}

int TextEditorSelectionSystem::GetCursorPosition(GameObject* object, Vector3 worldPosition) {
    TextEditor* textEditor = object->GetComponent<TextEditor>();
    Transform* transform = object->GetComponent<Transform>();
    TextEditorRenderer* renderer = object->GetComponent<TextEditorRenderer>();
    Font* font = object->GetComponent<Font>();
    Vector2 size = object->GetComponent<Sizeable>()->Size;
    
    Vector3 localPosition = transform->WorldInverse().TransformPosition(worldPosition);
    
    float fontSize = renderer->fontSize;
    float spacing = font->GetSpacing(fontSize);
    
    Vector2 scaler = { spacing, renderer->fontSize };
    
    Point start = 0;
    
    Point cursor;
    
    //localPosition.x = -renderer->Offset().x + cursor.x * scaler.x;
    cursor.x = (int)roundf((localPosition.x + renderer->Offset().x) / scaler.x);
    cursor.y = (int)roundf((localPosition.y - renderer->Offset().y - size.y + fontSize) / -scaler.y);
    return textEditor->CartesianToCursor(cursor);
}

void TextEditorSelectionSystem::Update(float dt) {
    if (!isDown) return;
    const Vector2& touchPosition = downObject.touchData.Input->GetTouchPosition(0);
    Ray ray = downObject.touchData.Camera->GetRay(downObject.touchData.CameraTransform, touchPosition);
    Vector3 worldPosition = ray.GetPosition(downObject.touchData.WorldPosition.z);
    
    TextEditor* textEditor = downObject.object->GetComponent<TextEditor>();
    Point selection = textEditor->Selection;
    
    selection.y = GetCursorPosition(downObject.object, worldPosition);
    
    textEditor->Selection = selection;
    textEditor->SelectionActive = true;
    
    textEditor->Cursor = selection.y;
}
