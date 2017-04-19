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
                
                std::string templateParameters;
            };
        
            struct Result {
                int cursorKind;
                std::vector<ResultChunk> chunks;
                
                std::string GetLowerCaseText() {
                    for(auto& c : chunks) {
                        if (c.kind == 1) {
                            std::string lower = c.text;
                            std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
                            return lower;
                        }
                    }
                    return "";
                }
                
                std::string GetText() {
                    for(auto& c : chunks) {
                        if (c.kind == 1) {
                            return c.text;
                        }
                    }
                    return "";
                }
                
                std::string GetTemplateText() {
                    std::string templates = "";
                    int templateSwitch = 0;
                    for(auto& c : chunks) {
                        if (c.kind == 12) {
                            templateSwitch++;
                        }
                        if (templateSwitch == 1) {
                            templates += c.text;
                        }
                        if(c.kind == 13) {
                            templateSwitch++;
                        }
                    }
                    return templates;
                }
                
                std::string GetDisplayedText() {
                    return GetText()+GetTemplateText();
                }
                
                std::string GetReturnType() {
                    for(auto& c : chunks) {
                        if (c.kind == 15) {
                            return c.text;
                        }
                    }
                    return "";
                }

            };
        
            std::vector<Result> AutoCompleteFile(const std::string& file, const std::string& unsavedStr, int lineNo, int columnNo);
    };
}
