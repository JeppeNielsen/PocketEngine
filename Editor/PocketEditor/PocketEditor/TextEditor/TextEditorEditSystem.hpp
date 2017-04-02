//
//  TextEditorEditSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 29/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "TextEditor.hpp"
#include "TextBox.hpp"

using namespace Pocket;

class TextEditorEditSystem : public GameSystem<TextEditor, TextBox> {
protected:
    void Initialize() override;
    void Destroy() override;
    void ObjectAdded(GameObject* object) override;
    void ObjectRemoved(GameObject* object) override;
private:
    void TextBoxActivated(GameObject* object);
    void ButtonDown(std::string button);
    GameObject* activeTextEditor;
};
