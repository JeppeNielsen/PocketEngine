//
//  AutocompleterSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 09/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Autocompleter.hpp"
#include "AutocompleterEntry.hpp"

using namespace Pocket;

struct AutocompleterEntrySystem : public GameSystem<Autocompleter, AutocompleterEntry> {
    void ObjectAdded(GameObject* object) override;
    void ObjectRemoved(GameObject* object) override;
    void AutoCompleteDone(std::vector<Autocompleter::Result> results, GameObject* object);
};
