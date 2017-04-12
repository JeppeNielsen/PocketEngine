//
//  TextEditorScrollSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 06/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "TextEditor.hpp"
#include "TextBox.hpp"
#include "TextEditorRenderer.hpp"

using namespace Pocket;

class TextEditorScrollSystem : public GameSystem<TextEditor, TextBox, TextEditorRenderer> {
protected:
    void Initialize() override;
    void Destroy() override;
    void ObjectAdded(GameObject* object) override;
    void ObjectRemoved(GameObject* object) override;
    void Update(float dt) override;
private:
    void TextBoxActivated(GameObject* object);
    void OffsetChanged(GameObject* object);
    void InputScrollChanged(float scrollAmount);
    GameObject* activeTextEditor;
    bool scrollMovedFromHere;
    bool isScrolling;
    Vector2 scrollTarget;
};
