//
//  AutocompleterSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 09/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "AutocompleterEntrySystem.hpp"
#include <algorithm>

void AutocompleterEntrySystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Autocompleter>()->OnAutoComplete.Bind(this, &AutocompleterEntrySystem::AutoCompleteDone, object);
}

void AutocompleterEntrySystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Autocompleter>()->OnAutoComplete.Unbind(this, &AutocompleterEntrySystem::AutoCompleteDone, object);
}

void AutocompleterEntrySystem::AutoCompleteDone(std::vector<Autocompleter::Result> results, Pocket::GameObject *object) {
    
    //std::cout << "AutoCompleteDone" << std::endl;
    
    for(auto child : object->Children()) {
        child->Remove();
    }
    
    std::sort(results.begin(), results.end(), [] (auto& a, auto& b) {
        return a.GetLowerCaseText() < b.GetLowerCaseText();
    });
    
    for(auto& result : results) {
        if (result.cursorKind == 24) continue; //contructor
        if (result.cursorKind == 25) continue; //destructor
        if (result.cursorKind == 8) continue; //functions
        if (result.availability != 0) continue; // not available, ie private or protected
        
        GameObject* entryGO = object->CreateChild();
        AutocompleterEntry* entry = entryGO->AddComponent<AutocompleterEntry>();
        
        entry->entry = result;
    }
}
