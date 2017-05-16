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
#include "Event.hpp"
#include <clang-c/Index.h>

struct Autocompleter {
    struct ResultChunk {
        int kind;
        std::string text;
        
        std::string templateParameters;
    };

    struct Result {
        int cursorKind;
        int availability;
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

    std::vector<Result> DoAutoComplete(CXTranslationUnit translationUnit, const std::string& filename, const std::string& unsavedStr, int lineNo, int columnNo);
    Pocket::Event<std::vector<Result>> OnAutoComplete;
};

