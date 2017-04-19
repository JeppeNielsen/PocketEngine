//
//  Autocompleter.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 08/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//


#include "Autocompleter.hpp"
#include "ScriptAutoCompleter.hpp"

using namespace Pocket;

std::vector<Pocket::ScriptAutoCompleter::Result> Autocompleter::DoAutoComplete(const std::string& unsavedStr, int lineNo, int columnNo) {
    ScriptAutoCompleter completer;
    auto results = completer.AutoCompleteFile(sourceFile, unsavedStr, lineNo, columnNo);
    OnAutoComplete(results);
    return results;
}
