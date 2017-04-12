//
//  AutocompleterTextEditorSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 09/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Autocompleter.hpp"
#include "TextBox.hpp"
#include "TextEditor.hpp"

using namespace Pocket;

namespace Pocket {
    class Gui;
}

class AutocompleterTextEditorSystem : public GameSystem<Autocompleter, TextEditor, TextBox> {
protected:
    void Initialize() override;
    void Destroy() override;
    void ObjectAdded(GameObject* object) override;
    void ObjectRemoved(GameObject* object) override;
private:
    void TextBoxActivated(GameObject* object);
    void ButtonDown(ButtonEvent e);
    GameObject* activeTextEditor;
    GameObject* listBox;
    Pocket::Gui* gui;
};
