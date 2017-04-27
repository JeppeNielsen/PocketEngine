//
//  TextEditorSyntaxHighlightingSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 23/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "TextEditorSyntaxHighlightingSystem.hpp"

void TextEditorSyntaxHighlightingSystem::Initialize() {
    
}

void TextEditorSyntaxHighlightingSystem::ObjectAdded(Pocket::GameObject *object) {
    //object->GetComponent<TextEditor>()->Lines.HasBecomeDirty.Bind(this, &TextEditorSyntaxHighlightingSystem::LinesChanged, object);
    object->GetComponent<CodeFile>()->TranslationUnitUpdated.Bind(this, &TextEditorSyntaxHighlightingSystem::LinesChanged, object);
    object->GetComponent<TextEditor>()->TextInserted.Bind(this, &TextEditorSyntaxHighlightingSystem::TextInserted, object);
}

void TextEditorSyntaxHighlightingSystem::ObjectRemoved(Pocket::GameObject *object) {
    //object->GetComponent<TextEditor>()->Lines.HasBecomeDirty.Unbind(this, &TextEditorSyntaxHighlightingSystem::LinesChanged, object);
    object->GetComponent<CodeFile>()->TranslationUnitUpdated.Unbind(this, &TextEditorSyntaxHighlightingSystem::LinesChanged, object);
    object->GetComponent<TextEditor>()->TextInserted.Unbind(this, &TextEditorSyntaxHighlightingSystem::TextInserted, object);
}

void TextEditorSyntaxHighlightingSystem::Update(float dt) {
    worker.Update();
}

CXChildVisitResult TextEditorSyntaxHighlightingSystem_visitor(CXCursor cursor, CXCursor parent, CXClientData clientData) {
    
    TextEditorSyntaxHighlightingSystem::Tokens* tokens = (TextEditorSyntaxHighlightingSystem::Tokens*)clientData;
    
    
    
    if (cursor.kind == CXCursor_ClassDecl || cursor.kind == CXCursor_StructDecl || cursor.kind == CXCursor_FieldDecl) {
    
        CXCursor typeCursor = clang_getTypeDeclaration(clang_getCursorType(cursor));
     
        CXSourceRange range = clang_Cursor_getSpellingNameRange(typeCursor,0,0);
        CXSourceLocation startLocation = clang_getRangeStart(range);
        CXSourceLocation endLocation = clang_getRangeEnd(range);
    
        TextEditorSyntaxHighlightingSystem::Token token;
        CXFile file;
        clang_getFileLocation(startLocation, &file, &token.startLine, &token.startColumn, 0);
        clang_getFileLocation(endLocation, 0, &token.endLine, &token.endColumn, 0);
        
        CXString fileName = clang_getFileName(file);
        
        token.filename = std::string(clang_getCString(fileName));
        /*
        CXString cursorString = clang_getCursorSpelling(cursor);
        
        std::string spelling = std::string(clang_getCString(cursorString));
        
        token.length = (int)spelling.length();
        */
        
        token.color = Colour::Green();
        
        tokens->push_back(token);
        
        return CXChildVisit_Recurse;
    }
    
    return CXChildVisit_Continue;
}

std::ostream& operator<<(std::ostream& stream, const CXString& str)
{
  stream << clang_getCString(str);
  clang_disposeString(str);
  return stream;
}

std::string GetClangString(CXString s) {
    std::string str(clang_getCString(s));
    clang_disposeString(s);
    return str;
}

void TextEditorSyntaxHighlightingSystem::LinesChanged(Pocket::GameObject *object) {
/*
    CodeFile* codeFile = object->GetComponent<CodeFile>();
        
        Tokens tokens;
        
        CXCursor startCursor = clang_getTranslationUnitCursor(codeFile->translationUnit);
        //clang_visitChildren(startCursor, TextEditorSyntaxHighlightingSystem_visitor, &tokens);
        
        
        clang_visitChildren(
        startCursor,
        [](CXCursor c, CXCursor parent, CXClientData client_data)
        {
            
            clang_getTypeDeclaration(<#CXType T#>)
            
        
          std::cout << "Cursor '" << clang_getCursorSpelling(c) << "' of kind '"
            << clang_getCursorKindSpelling(clang_getCursorKind(c)) << "'"
            << "type :"<< clang_getTypeSpelling(clang_getCursorType(c))<<std::endl;
            
          return CXChildVisit_Recurse;
        },
        nullptr);

return;
*/

    if (workingObjects.find(object)!=workingObjects.end()) return;
    workingObjects.insert(object);
        CodeFile* codeFile = object->GetComponent<CodeFile>();
    
    worker.DoTask([object, codeFile] () {
    
        while (codeFile->IsTranslationUnitInUse) {
            std::this_thread::sleep_for(std::chrono::nanoseconds(1000000));
        }
        codeFile->IsTranslationUnitInUse = true;
        
        Tokens tokens;
        
        CXCursor startCursor = clang_getTranslationUnitCursor(codeFile->translationUnit);
        clang_visitChildren(startCursor, [] (CXCursor cursor, CXCursor parent, CXClientData clientData) {
    
        TextEditorSyntaxHighlightingSystem::Tokens* tokens = (TextEditorSyntaxHighlightingSystem::Tokens*)clientData;
    
        if (cursor.kind == CXCursor_ClassDecl || cursor.kind == CXCursor_StructDecl
            || cursor.kind == CXCursor_FieldDecl
            || cursor.kind == CXCursor_CXXMethod
            ) {
        
            //std::string typeSpelling = GetClangString(clang_getTypeSpelling(clang_getCursorType(cursor)));
            //CXCursor typeCursor = clang_getRan);
            
            
            //if (typeCursor.kind != CXCursor_NoDeclFound) {
                CXSourceRange range = clang_Cursor_getSpellingNameRange(cursor,0,0);
                CXSourceLocation startLocation = clang_getRangeStart(range);
                CXSourceLocation endLocation = clang_getRangeEnd(range);
                
                TextEditorSyntaxHighlightingSystem::Token token;
                CXFile file;
                clang_getFileLocation(startLocation, &file, &token.startLine, &token.startColumn, 0);
                clang_getFileLocation(endLocation, 0, &token.endLine, &token.endColumn, 0);
            
            
                CXString fileName = clang_getFileName(file);
                token.filename = std::string(clang_getCString(fileName));
            
                /*
                CXString cursorString = clang_getCursorSpelling(cursor);
                
                std::string spelling = std::string(clang_getCString(cursorString));
                
                token.length = (int)spelling.length();
                */
                
                token.color = Colour::Blue();
                
                tokens->push_back(token);
            }
        //}
    
        return CXChildVisit_Recurse;
    }, &tokens);
        return tokens;
    }, [object, this] (Tokens tokens) {
        
        CodeFile* codeFile = object->GetComponent<CodeFile>();
        codeFile->IsTranslationUnitInUse = false;
        
        workingObjects.erase(workingObjects.find(object));
        
        TextEditorColorer* colorer = object->GetComponent<TextEditorColorer>();
        TextEditor* textEditor = object->GetComponent<TextEditor>();
        
        colorer->colors.clear();
        
        for(auto& t : tokens) {
            if (t.filename!=codeFile->path) continue;
            int start = textEditor->CartesianToCursor({(int)t.startColumn-1, (int)t.startLine-1});
            int end = textEditor->CartesianToCursor({(int)t.endColumn-1, (int)t.endLine-1});
            colorer->AddColor(start, end, t.color);
        }
    });
}

void TextEditorSyntaxHighlightingSystem::TextInserted(Point offset, Pocket::GameObject *object) {
    object->GetComponent<TextEditorColorer>()->MoveNodes(offset.x, offset.y);
}
