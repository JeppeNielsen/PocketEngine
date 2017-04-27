//
//  AutocompleterSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 19/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "AutocompleterSystem.hpp"
#include "ScriptAutoCompleter.hpp"


void AutocompleterSystem::Initialize() {

}

void AutocompleterSystem::Destroy() {
    
}

void AutocompleterSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<TextEditor>()->AutoCompleteEnabled.Bind(this, &AutocompleterSystem::OnAutoCompleteEnabled, object);
}

void AutocompleterSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<TextEditor>()->AutoCompleteEnabled.Unbind(this, &AutocompleterSystem::OnAutoCompleteEnabled, object);
}

void AutocompleterSystem::OnAutoCompleteEnabled(Pocket::GameObject *object) {
    TextEditor* textEditor = object->GetComponent<TextEditor>();
    Point cursor = textEditor->CursorToCartesian(textEditor->AutoCompleteCursor);
    Autocompleter* autocompleter = object->GetComponent<Autocompleter>();
    CodeFile* codeFile = object->GetComponent<CodeFile>();
    CXTranslationUnit tu = codeFile->translationUnit;
    worker.DoTask([=] () {
        while (codeFile->IsTranslationUnitInUse) {
            std::this_thread::sleep_for(std::chrono::nanoseconds(1000000));
        }
        codeFile->IsTranslationUnitInUse = true;
        return autocompleter->DoAutoComplete(tu, codeFile->path, textEditor->text, cursor.y + 1, cursor.x + 1);
    }, [=] (std::vector<Autocompleter::Result> results) {
        autocompleter->OnAutoComplete(results);
        codeFile->IsTranslationUnitInUse = false;
    });
}

void AutocompleterSystem::Update(float dt) {
    worker.Update();
}
