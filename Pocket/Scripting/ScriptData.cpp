//
//  ScriptData.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 15/05/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "ScriptData.hpp"
#include <clang-c/Index.h>
#include <iostream>
#include "StringHelper.hpp"
#include <set>

using namespace Pocket;

void ScriptData::Clear() {
    components.clear();
    systems.clear();
}

CXChildVisitResult parseCode(CXCursor cursor, CXCursor parent, CXClientData clientData);

bool ScriptData::Parse(const std::vector<std::string> &cppFiles, const std::vector<std::string> &includePaths, const std::function<bool(const std::string&)>& predicate) {
    std::vector<const char*> arguments;
    arguments.push_back("c++");
    arguments.push_back("-std=c++11");
    arguments.push_back("-stdlib=libc++");
    arguments.push_back("-I/Users/Jeppe/Downloads/clang+llvm-3.7.0-x86_64-apple-darwin/include/c++/v1");
    arguments.push_back("-I/usr/include");
    arguments.push_back("-I/usr/include/c++/4.2.1/");
    arguments.push_back("-I/Users/Jeppe/Downloads/clang+llvm-3.7.0-x86_64-apple-darwin/lib/clang/3.7.0/include");
    arguments.push_back("-I/Projects/PocketEngine/Pocket/Data/");
    
    for(auto& includePath : includePaths) {
        arguments.push_back(includePath.c_str());
    }
    
    for(auto cppFile : cppFiles) {
        CXIndex index = clang_createIndex(0,1);

        // create Translation Unit
        CXTranslationUnit tu = clang_parseTranslationUnit(index, cppFile.c_str(), &arguments[0], (int)arguments.size(), NULL, 0, 0);
        if (tu == NULL) {
            printf("Cannot parse translation unit\n");
            return false;
        }

        CXCursor startCursor = clang_getTranslationUnitCursor(tu);
        clang_visitChildren(startCursor, parseCode, this);
    }
    
    std::set<std::string> uniqueComponentNames;
    
    for(auto& s : systems) {
        for(auto& c : s.components) {
            auto found = c.find("struct ");
            if (found!=std::string::npos) {
                c = c.substr(0, found) + c.substr(found + 7, c.size() - found - 7);
            } else {
                found = c.find("class ");
                if (found!=std::string::npos) {
                    c = c.substr(0, found) + c.substr(found + 6, c.size() - found - 6);
                }
            }
            uniqueComponentNames.insert(c);
        }
    }
    
    Components systemComponents;
    
    for(auto& componentName : uniqueComponentNames) {
        Component* component = FindComponent(componentName);
        if (!component) continue;
        if (!predicate(componentName)) continue;
        systemComponents.push_back(*component);
    }
    
    components = systemComponents;
    
    return true;
}

ScriptData::Component* ScriptData::FindComponent(const std::string &name) {
    for(auto& c : components) {
        if (c.name == name) return &c;
    }
    return nullptr;
}

std::string getCursorSpelling2( CXCursor cursor )
{
  CXString cursorSpelling = clang_getCursorSpelling( cursor );
  std::string result      = clang_getCString( cursorSpelling );

  clang_disposeString( cursorSpelling );
  return result;
}

std::string getCursorType( CXCursor cursor )
{
  CXType type = clang_getCursorType( cursor );
  CXString cursorSpelling = clang_getTypeSpelling(type);

  std::string result      = clang_getCString( cursorSpelling );

  clang_disposeString( cursorSpelling );
  return result;
}

CXChildVisitResult parseCode(CXCursor cursor, CXCursor parent, CXClientData clientData) {
    CXSourceLocation location = clang_getCursorLocation( cursor );
    if (clang_Location_isInSystemHeader(location) != 0) {
        return CXChildVisit_Continue;
    }
    
    ScriptData& scriptData = *static_cast<ScriptData*>(clientData);
    
    std::string cursorSpelling = getCursorSpelling2(cursor);
    
    static bool isSystem = false;
    static std::string nameSpace = "";
    
    if (parent.kind == CXCursor_TranslationUnit) {
        nameSpace = "";
        isSystem = false;
    }
    
    if (cursor.kind == CXCursor_Namespace) {
        nameSpace = cursorSpelling;
        isSystem = false;
    } else if (cursor.kind == CXCursor_StructDecl || cursor.kind == CXCursor_ClassDecl) {
        
        ScriptData::Component component;
        if (nameSpace!="") {
            component.name = nameSpace +"::" + cursorSpelling;
        } else {
            component.name = cursorSpelling;
        }
        
        scriptData.components.push_back(component);
        
        isSystem = false;
    } else if (cursor.kind == CXCursor_TemplateRef && cursorSpelling == "GameSystem") {
        scriptData.systems.push_back({scriptData.components.back().name});
        isSystem = true;
    } else if (cursor.kind == CXCursor_TypeRef && isSystem && parent.kind == CXCursor_CXXBaseSpecifier) {
        scriptData.systems.back().components.push_back(cursorSpelling);
    } else if (cursor.kind == CXCursor_FieldDecl && !scriptData.components.empty()) {
        isSystem = false;
    
        //std::cout << scriptData.components.back().name<<":" << cursorSpelling << " : " << getCursorType(cursor) << std::endl;
        scriptData.components.back().fields.push_back({cursorSpelling, getCursorType(cursor)});
    }
    
    return CXChildVisit_Recurse;
}
