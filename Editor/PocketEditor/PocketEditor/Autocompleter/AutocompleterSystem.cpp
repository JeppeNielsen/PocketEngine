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
    //thread(&AutocompleterSystem::ThreadLoop);
    thread = std::thread(&AutocompleterSystem::ThreadLoop, this);
    anythingToProcess = false;
    anythingProcessed = false;
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

    ObjectToProcess obj = {
        autocompleter,
        cursor.y + 1,
        cursor.x + 1,
        autocompleter->sourceFile,
        textEditor->text
    }; 

    mutex.lock();
    objectsToProcess.clear();
    objectsToProcess.emplace_back(obj);
    mutex.unlock();
    
    anythingToProcess = true;
}

void AutocompleterSystem::Update(float dt) {
    if (!anythingProcessed) return;
    anythingProcessed = false;
    mutex.lock();
    for(auto& o : objectsProcessed) {
        o.autocompleter->results = o.results;
        o.autocompleter->OnAutoComplete(o.results);
    }
    objectsProcessed.clear();
    mutex.unlock();
}

void AutocompleterSystem::ThreadLoop() {
    
    ScriptAutoCompleter autoCompleter;
    
    while (true) {
        while (!anythingToProcess) {
            std::this_thread::sleep_for(std::chrono::nanoseconds(100000));
        }
        anythingToProcess = false;
        std::vector<ObjectToProcess> objectsToProcess;
        mutex.lock();
        objectsToProcess = this->objectsToProcess;
        this->objectsToProcess.clear();
        mutex.unlock();
    
        for(auto& o : objectsToProcess) {
            ObjectProcessed obj = {
                o.autoCompleter,
                autoCompleter.AutoCompleteFile(o.file, o.fileContent, o.line, o.column)
            };
            mutex.lock();
            anythingProcessed = true;
            objectsProcessed.emplace_back(obj);
            mutex.unlock();
        }
    }
}
