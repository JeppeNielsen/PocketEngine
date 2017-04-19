//
//  AutocompleterSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 19/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Autocompleter.hpp"
#include "TextEditor.hpp"
#include <set>
#include <thread>

using namespace Pocket;

struct AutocompleterSystem : public GameSystem<Autocompleter, TextEditor> {
    
    void Initialize() override;
    void Destroy() override;
    
    void ObjectAdded(GameObject* object) override;
    void ObjectRemoved(GameObject* object) override;
    void OnAutoCompleteEnabled(GameObject* object);
    void Update(float dt) override;
private:
    
    struct ObjectToProcess {
        Autocompleter* autoCompleter;
        int line;
        int column;
        std::string file;
        std::string fileContent;
    };
    
    std::vector<ObjectToProcess> objectsToProcess;
    
    struct ObjectProcessed {
        Autocompleter* autocompleter;
        decltype(Autocompleter::results) results;
    };
    
    std::vector<ObjectProcessed> objectsProcessed;
    
    void ThreadLoop();
    
    bool anythingToProcess;
    bool anythingProcessed;
    
    std::thread thread;
    std::mutex mutex;
};
