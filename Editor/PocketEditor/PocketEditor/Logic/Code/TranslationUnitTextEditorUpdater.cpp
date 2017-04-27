//
//  TranslationUnitTextEditorUpdater.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 25/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "TranslationUnitTextEditorUpdater.hpp"

void TranslationUnitTextEditorUpdater::LinesChanged(GameObject* object) {
    CodeFile* codeFile = object->GetComponent<CodeFile>();
    TextEditor* textEditor = object->GetComponent<TextEditor>();
    std::string text = textEditor->text;
    
    if (workingObjects.find(object)!=workingObjects.end()) return;
    workingObjects.insert(object);

    worker.DoTask([codeFile, text] () {
        while (codeFile->IsTranslationUnitInUse) {
            std::this_thread::sleep_for(std::chrono::nanoseconds(1000000));
        }
        codeFile->IsTranslationUnitInUse = true;
        return codeFile->ReparseTranslationUnit(text);
    }, [codeFile, object, this] (bool succes) {
        if (succes) {
            codeFile->TranslationUnitUpdated();
        }
        codeFile->IsTranslationUnitInUse = false;
        workingObjects.erase(workingObjects.find(object));
    });
}

void TranslationUnitTextEditorUpdater::ObjectAdded(Pocket::GameObject *object) {
    //object->GetComponent<TextEditor>()->Lines.HasBecomeDirty.Bind(this, &TranslationUnitTextEditorUpdater::LinesChanged, object);
}

void TranslationUnitTextEditorUpdater::ObjectRemoved(Pocket::GameObject *object) {
    //object->GetComponent<TextEditor>()->Lines.HasBecomeDirty.Unbind(this, &TranslationUnitTextEditorUpdater::LinesChanged, object);
}

void TranslationUnitTextEditorUpdater::Update(float dt) {
    worker.Update();
}
