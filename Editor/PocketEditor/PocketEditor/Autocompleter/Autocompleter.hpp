//
//  Autocompleter.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 08/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <functional>
#include "ScriptAutoCompleter.hpp"
#include "Event.hpp"

struct Autocompleter {
    std::string sourceFile;
    std::vector<Pocket::ScriptAutoCompleter::Result> DoAutoComplete(const std::string& unsavedStr, int lineNo, int columnNo);
    Pocket::Event<std::vector<Pocket::ScriptAutoCompleter::Result>> OnAutoComplete;
    
    std::vector<Pocket::ScriptAutoCompleter::Result> results;
};

