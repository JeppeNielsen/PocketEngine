//
//  CodeFile.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 23/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "CodeFile.hpp"

CodeFile::CodeFile() : translationUnit(0), IsTranslationUnitInUse(false) {}
CodeFile::CodeFile(const CodeFile& other) : translationUnit(0), IsTranslationUnitInUse(false) {

}

CodeFile& CodeFile::operator=(const CodeFile &other){
    translationUnit = 0;
    IsTranslationUnitInUse = false;
    return *this;
}

CXTranslationUnit CodeFile::ParseTranslationUnit() {

   /* if (translationUnit) {
        clang_disposeTranslationUnit(translationUnit);
    }
    */

    std::vector<const char*> arguments;
    arguments.push_back("c++");
    arguments.push_back("-std=c++11");
    arguments.push_back("-stdlib=libc++");
    
    arguments.push_back("-I/Users/Jeppe/Downloads/clang+llvm-3.7.0-x86_64-apple-darwin/include/c++/v1");
    arguments.push_back("-I/Users/Jeppe/Downloads/clang+llvm-3.7.0-x86_64-apple-darwin/lib/clang/3.7.0/include");
    
    //arguments.push_back("-I/usr/include");
    //arguments.push_back("-I/usr/include/c++/4.2.1/");
    arguments.push_back("-I/Projects/PocketEngine/Pocket/Data/");
    arguments.push_back("-I/Projects/PocketEngine/Pocket/ComponentSystem/");
    arguments.push_back("-I/Projects/PocketEngine/Pocket/ComponentSystem/Meta/");
    
    CXIndex index = clang_createIndex(0,1);
    CXTranslationUnit tu  = clang_parseTranslationUnit(index, path.c_str(), &arguments[0], (int)arguments.size(), NULL, 0, 0);
    
    return tu;
}

bool CodeFile::ReparseTranslationUnit(const std::string& code) {
    if (!translationUnit) return false;
    CXUnsavedFile file;
    file.Filename = path.c_str();
    file.Contents = code.c_str();
    file.Length = code.size();
    return clang_reparseTranslationUnit(translationUnit, 1, &file, clang_defaultReparseOptions(translationUnit)) == 0;
}
