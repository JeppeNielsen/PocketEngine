//
//  TextEditorCursorSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 29/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "TextEditor.hpp"
#include "TextBox.hpp"
#include "TextEditorRenderer.hpp"
#include "Sizeable.hpp"
#include "Font.hpp"

using namespace Pocket;

class TextEditorCursorSystem : public GameSystem<TextEditor, TextBox, TextEditorRenderer, Sizeable, Font> {
protected:
    void Initialize() override;
    void Destroy() override;
    void ObjectAdded(GameObject* object) override;
    void ObjectRemoved(GameObject* object) override;
    void Update(float dt) override;
private:
    void TextBoxActivated(GameObject* object);
    void CursorChanged(GameObject* object);
    void OffsetChanged(GameObject* object);
    void UpdateCursorTransform();
    Vector2 cursorPosition;
    GameObject* cursorObject;
    GameObject* activeTextEditor;
    Vector2 scrollTarget;
    bool isScrolling;
    bool scrollMovedFromHere;
};
