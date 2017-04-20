//
//  ScriptAutoCompleter.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 07/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "ScriptAutoCompleter.hpp"
#include <iostream>
#include <iomanip>
#include "ClangUtil.hpp"
#include <clang-c/Index.h>
#include <vector>

using namespace Pocket;

std::string ChunkKindToString(CXCompletionChunkKind kind) {
    switch (kind)  {
        case CXCompletionChunk_Optional: return "CXCompletionChunk_Optional";
        case CXCompletionChunk_TypedText: return "CXCompletionChunk_TypedText";
        case CXCompletionChunk_Text: return "CXCompletionChunk_Text";
        case CXCompletionChunk_Placeholder: return "CXCompletionChunk_Placeholder";
        case CXCompletionChunk_Informative: return "CXCompletionChunk_Informative";
        case CXCompletionChunk_CurrentParameter: return "CXCompletionChunk_CurrentParameter";
        case CXCompletionChunk_LeftParen: return "CXCompletionChunk_LeftParen";
        case CXCompletionChunk_RightParen: return "CXCompletionChunk_RightParen";
        case CXCompletionChunk_LeftBracket: return "CXCompletionChunk_LeftBracket";
        case CXCompletionChunk_RightBracket: return "CXCompletionChunk_RightBracket";
        case CXCompletionChunk_LeftBrace: return "CXCompletionChunk_LeftBrace";
        case CXCompletionChunk_RightBrace: return "CXCompletionChunk_RightBrace";
        case CXCompletionChunk_LeftAngle: return "CXCompletionChunk_LeftAngle";
        case CXCompletionChunk_RightAngle: return "CXCompletionChunk_RightAngle";
        case CXCompletionChunk_Comma: return "CXCompletionChunk_Comma";
        case CXCompletionChunk_ResultType: return "CXCompletionChunk_ResultType";
        case CXCompletionChunk_Colon: return "CXCompletionChunk_Colon";
        case CXCompletionChunk_SemiColon: return "CXCompletionChunk_SemiColon";
        case CXCompletionChunk_Equal: return "CXCompletionChunk_Equal";
        case CXCompletionChunk_HorizontalSpace: return "CXCompletionChunk_HorizontalSpace";
        case CXCompletionChunk_VerticalSpace: return "CXCompletionChunk_VerticalSpace";
        default:
            return "undefined";
    }
}

std::string ChunkAvailablilityToString(CXAvailabilityKind availablility) {
    switch (availablility)  {
        case CXAvailability_Available: return "CXAvailability_Available";
        case CXAvailability_Deprecated: return "CXAvailability_Deprecated";
        case CXAvailability_NotAvailable: return "CXAvailability_NotAvailable";
        case CXAvailability_NotAccessible: return "CXAvailability_NotAccessible";
        default:
            return "undefined";
    }
}

void PrintCompletionString(CXCompletionString string, int indent) {

    int numCompletionChunks = clang_getNumCompletionChunks(string);
    for(int i=0; i<numCompletionChunks; i++) {
            
        CXCompletionChunkKind kind = clang_getCompletionChunkKind(string, i);
        for(int in = 0; in<indent; in++) {
            std::cout <<" ";
        }
        std::cout <<"CXCompletionChunkKind: "<< kind << " " << ChunkKindToString(kind) << std::endl;
        
        for(int in = 0; in<indent; in++) {
            std::cout <<" ";
        }
        CXString chuckText = clang_getCompletionChunkText(string, i);
        std::cout <<"text:"<< clang_getCString(chuckText) << std::endl;
        
        CXAvailabilityKind availablilty = clang_getCompletionAvailability(string);
        
        std::cout <<"CXAvailabilityKind: "<< availablilty << " " << ChunkAvailablilityToString(availablilty) << std::endl;
        
    
        CXCompletionString completionString = clang_getCompletionChunkCompletionString(string, i);
        PrintCompletionString(completionString, indent + 3);
    }
}

std::vector<ScriptAutoCompleter::Result> ScriptAutoCompleter::AutoCompleteFile(const std::string &file, const std::string& unsavedStr, int lineNo, int columnNo) {

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
    
    

    // create Translation Unit
    static CXTranslationUnit tu = 0;
    
    if (!tu) {
        CXIndex index = clang_createIndex(0,1);
        tu  = clang_parseTranslationUnit(index, file.c_str(), &arguments[0], (int)arguments.size(), NULL, 0, 0);
        if (tu == NULL) {
            printf("Cannot parse translation unit\n");
            return {};
        }
    }
    
    std::vector<ScriptAutoCompleter::Result> results;
    
    CXUnsavedFile unsavedFile;
    unsavedFile.Filename = file.c_str();
    unsavedFile.Contents = unsavedStr.c_str();
    unsavedFile.Length = unsavedStr.size();
    
    CXCodeCompleteResults* completeResults = clang_codeCompleteAt(tu, file.c_str(), lineNo, columnNo, &unsavedFile, 1, 0);
    
    if (completeResults) {
        
         for (int i=0; i<completeResults->NumResults; ++i) {
            CXCompletionResult completionResult = completeResults->Results[i];
            if (completionResult.CursorKind == CXCursor_NotImplemented) continue;
            
            //std::cout <<"CXCursorKind: "<<completionResult.CursorKind<<std::endl;
            
            //PrintCompletionString(completionResult.CompletionString , 3);
            
            Result result;
            result.cursorKind = completionResult.CursorKind;
            result.availability = clang_getCompletionAvailability(completionResult.CompletionString);
            
            int angleBracket = 0;
            
            int numCompletionChunks = clang_getNumCompletionChunks(completionResult.CompletionString);
            for(int chunkNo=0; chunkNo<numCompletionChunks; chunkNo++) {
                
                int kind = clang_getCompletionChunkKind(completionResult.CompletionString, chunkNo);
                
                if (kind == CXCompletionChunk_LeftAngle || kind == CXCompletionChunk_RightAngle) {
                    angleBracket++;
                }
                
                ResultChunk chunk;
                
                chunk.kind = kind;
                
                CXString chuckText = clang_getCompletionChunkText(completionResult.CompletionString, chunkNo);
                chunk.text = std::string(clang_getCString(chuckText));
            
                result.chunks.push_back(chunk);
                
                
            }
            
            results.push_back(result);
        }
        
        
        clang_disposeCodeCompleteResults(completeResults);
    }
    return results;
}
