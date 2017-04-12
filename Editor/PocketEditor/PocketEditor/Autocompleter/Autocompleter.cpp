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

void Autocompleter::DoAutoComplete(int lineNo, int columnNo) {
    ScriptAutoCompleter completer;
    auto results = completer.AutoCompleteFile(sourceFile, lineNo, columnNo);
    OnAutoComplete(results);
}
