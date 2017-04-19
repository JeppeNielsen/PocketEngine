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
#include "TextEditorRenderer.hpp"
#include "Font.hpp"
#include "Sizeable.hpp"

using namespace Pocket;

namespace Pocket {
    class Gui;
    class VirtualTreeList;
}

class AutocompleterEntry;

class AutocompleterTextEditorSystem : public GameSystem<Autocompleter, TextEditor, TextBox, TextEditorRenderer, Font, Sizeable> {
protected:
    void Initialize() override;
    void Destroy() override;
    void ObjectAdded(GameObject* object) override;
    void ObjectRemoved(GameObject* object) override;
private:
    void TextBoxActivated(GameObject* object);
    
    void AutoCompleteDisabled(GameObject* object);

    void OnAutoComplete(std::vector<Pocket::ScriptAutoCompleter::Result> results, GameObject* object);
    void RemoveListBox();
    void OnAutoCompleteEvent(TextEditor::AutoCompleteEventType eventType, GameObject* object);
    AutocompleterEntry* FindAutocompleterEntryByIndex(int index);
    
    GameObject* activeTextEditor;
    GameObject* listBox;
    Pocket::Gui* gui;
    Pocket::VirtualTreeList* treeView;
    GameObject* functionParametersListBox;
    int selectedTokenIndex;
};
