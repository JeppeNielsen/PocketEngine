//
//  ScriptAutoCompleter.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 07/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <functional>
#include <vector>

namespace Pocket {
    class ScriptAutoCompleter {
        public:
        
            struct ResultChunk {
                int kind;
                std::string text;
            };
        
            struct Result {
                int cursorKind;
                std::vector<ResultChunk> chunks;
            };
        
            std::vector<Result> AutoCompleteFile(const std::string& file, int lineNo, int columnNo);
    };
}
