//
//  CodeFile.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 23/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <clang-c/Index.h>
#include "Event.hpp"

using namespace Pocket;

struct CodeFile {
    CodeFile();
    CodeFile(const CodeFile& other);
    CodeFile& operator=(const CodeFile& other);
    std::string path;
    CXTranslationUnit translationUnit;
    Event<> TranslationUnitUpdated;
    
    CXTranslationUnit ParseTranslationUnit();
    bool ReparseTranslationUnit(const std::string& code);
    
    std::atomic<bool> IsTranslationUnitInUse;
};
