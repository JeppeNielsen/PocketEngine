//
//  testAutoComplete.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 07/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "ScriptAutoCompleter.hpp"
#include <iostream>

using namespace Pocket;

int main_autocompleter() {

    

    ScriptAutoCompleter completer;
    
    auto results = completer.AutoCompleteFile("/Projects/PocketEngine/EditorProjects/AutoComplete/main.cpp", "", 21, 7);
    


    return 0;
}
