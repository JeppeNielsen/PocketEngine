//
//  ScriptParser.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 31/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "ClangUtil.hpp"
#include <clang-c/Index.h>
#include "ScriptParser.hpp"
#include <fstream>
#include <set>
#include <algorithm>

using namespace std;
using namespace Pocket;

std::string
basename( std::string const& pathname )
{
    return std::string( 
        std::find_if( pathname.rbegin(), pathname.rend(),
                      [] (char ch) { return ch == '/'; } ).base(),
        pathname.end() );
}

string path = "";
string className = "";

static ScriptClass* currentClass;

CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData clientData) {
    //
    // For now, only want to process the main file of interest...
    //
    //if ( !clang_Location_isFromMainFile(clang_getCursorLocation(cursor)) )
        //return CXChildVisit_Continue;

    string cursorSpelling = toStdString(cursor);

    CXTranslationUnit tu = clang_Cursor_getTranslationUnit(cursor);
    CXSourceRange range = clang_getCursorExtent(cursor);
    
    if ( cursor.kind == CXCursor_ClassDecl || cursor.kind == CXCursor_StructDecl)
    {
        if ( className == "" || className == cursorSpelling )
        {
            ScriptClass* parent = currentClass;
        
            //cout << "Found class " << cursorSpelling << endl;
            
            currentClass = &currentClass->children[cursorSpelling];
            currentClass->parent = parent;
            currentClass->name = cursorSpelling;
            
            //
            // Now we'll tokenize the range, which encompasses the whole class,
            // and annotate it.
            //
            CXToken* tokens = 0;
            unsigned int numTokens;
            clang_tokenize(tu, range, &tokens, &numTokens);

            CXCursor cursors[numTokens];
            clang_annotateTokens(tu, tokens, numTokens, cursors);
            
            /*
            cout << std::left << setw(28) << "Token" << setw(28) << "Cursor" << setw(35) <<
                "Cursor Kind"  << setw(34) << "Cursor Type" << endl;
            cout << "=================================================================================" <<
                endl;
            */
            bool baseClassParsingStarted = false;
            bool templateArgumentsStarted = false;
            bool fileSet = false;
            currentClass->baseClass = "";
            
            for ( unsigned int idx=0; idx<numTokens; ++idx )
            {
                CXType type = clang_getCursorType(cursors[idx]);
                string cursorSpelling = toStdString(cursors[idx]);
                string tokenSpelling = toStdString(tokens[idx], tu);
                string typeSpelling = toStdString(type);
                string cursorKind = toStdString(cursors[idx].kind);
                
                if (currentClass->baseClass=="" && ((cursorKind == "StructDecl" || cursorKind == "ClassDecl") || cursorKind == "TemplateRef")) {
                    if (!baseClassParsingStarted) {
                        if (tokenSpelling == ":") {
                            baseClassParsingStarted = true;
                        }
                    } else {
                        std::string lowerCase = tokenSpelling;
                        std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), ::tolower);
                        if (lowerCase!="public" && lowerCase!="protected" && lowerCase!="private") {
                            currentClass->baseClass = tokenSpelling;
                        }
                    }
                }
                
                
                if (currentClass->baseClass!="" && tokenSpelling == ">") {
                    templateArgumentsStarted = false;
                }
                
                if (templateArgumentsStarted && (cursorKind == "TypeRef" || (cursorKind == "StructDecl" || cursorKind == "ClassDecl")) && tokenSpelling!=",") {
                    currentClass->templateArguments.push_back(tokenSpelling);
                }
                
                if (currentClass->baseClass!="" && tokenSpelling == "<") {
                    templateArgumentsStarted = true;
                }
                
                if (cursorKind == "FieldDecl" && cursorSpelling!=tokenSpelling) {
                    currentClass->fields.push_back({cursorSpelling, tokenSpelling });
                }
                
                if (!fileSet) {
                    CXSourceLocation loc = clang_getTokenLocation(tu, tokens[idx]);
    
                    CXFile file;
                    unsigned line, column, offset;
                    clang_getFileLocation(loc, &file, &line, &column, &offset);
                    CXString fileName = clang_getFileName(file);
                    
                    string sourceFilename = toStdString(fileName);
                    currentClass->sourceFile = basename(sourceFilename);
                    fileSet = true;
                }
                
                
                //cout << std::left << setw(28) << tokenSpelling << setw(28) << cursorSpelling <<
                  //  setw(35) << cursorKind << setw(34) <<  typeSpelling << endl;
            }
            //if ( numTokens > 0 )
                //cout << endl << endl;
            
            currentClass = parent;
        }
    }

    return CXChildVisit_Continue;
}




bool ScriptParser::ParseCode(ScriptClass& root, std::string cppFile, std::vector<std::string> includePaths) {

    std::vector<const char*> arguments;
    arguments.push_back("c++");
    arguments.push_back("-std=c++14");
    arguments.push_back("-stdlib=libc++");
    //arguments.push_back("-I/Users/Jeppe/Downloads/clang+llvm-3.7.0-x86_64-apple-darwin/include/c++/v1");
    //arguments.push_back("-I/usr/include");
    
    for(auto& includePath : includePaths) {
        arguments.push_back(includePath.c_str());
    }
    
    currentClass = &root;
    
    CXIndex index = clang_createIndex(0,1);

    // create Translation Unit
    CXTranslationUnit tu = clang_parseTranslationUnit(index, cppFile.c_str(), &arguments[0], (int)arguments.size(), NULL, 0, 0);
    if (tu == NULL) {
        printf("Cannot parse translation unit\n");
        return false;
    }

    CXCursor startCursor = clang_getTranslationUnitCursor(tu);
    clang_visitChildren(startCursor, visitor, 0);

    return true;
}

ScriptClass ScriptParser::ExtractSystemsAndComponents(const ScriptClass &root) {
    ScriptClass world;
    
    ScriptClass& systemsRoot = world.children["Systems"];
    auto& systems = systemsRoot.children;
    for (auto klass : root.children) {
        if (klass.second.baseClass == "GameSystem") {
            systems[klass.second.name] = klass.second;
        }
    }
    ScriptClass& componentsRoot = world.children["Components"];
    auto& components = componentsRoot.children;
    for (auto& system : systems) {
        for (auto& componentName : system.second.templateArguments) {
            auto it = components.find(componentName);
            if (it!=components.end()) {
                continue; // component already added to list, ignore
            }
            auto componentIt = root.children.find(componentName);
            if (componentIt == root.children.end()) {
                //std::cout << componentName <<" not found in script world"<< std::endl;
                continue;
            }
            components[componentName] = componentIt->second;
        }
    }
    return world;
}