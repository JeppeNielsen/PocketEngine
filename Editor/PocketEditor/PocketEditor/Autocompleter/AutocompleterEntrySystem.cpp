//
//  AutocompleterSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 09/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "AutocompleterEntrySystem.hpp"

void AutocompleterEntrySystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Autocompleter>()->OnAutoComplete.Bind(this, &AutocompleterEntrySystem::AutoCompleteDone, object);
}

void AutocompleterEntrySystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Autocompleter>()->OnAutoComplete.Unbind(this, &AutocompleterEntrySystem::AutoCompleteDone, object);
}

void AutocompleterEntrySystem::AutoCompleteDone(std::vector<Pocket::ScriptAutoCompleter::Result> results, Pocket::GameObject *object) {
    
    for(auto child : object->Children()) {
        child->Remove();
    }
    
    for(auto& result : results) {
        
        GameObject* entryGO = object->CreateChild();
        AutocompleterEntry* entry = entryGO->AddComponent<AutocompleterEntry>();
      
        for(auto& chunk : result.chunks) {
            if (chunk.kind == 1) {
                entry->text = chunk.text;
            } else if (chunk.kind == 15) {
                entry->returnType = chunk.text;
            }
        }
    }
    
}
