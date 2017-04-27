//
//  TextEditorSyntaxHighlightingSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 23/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameSystem.hpp"
#include "TextEditorColorer.hpp"
#include "CodeFile.hpp"
#include "TextEditor.hpp"
#include "Worker.hpp"
#include <set>

class TextEditorSyntaxHighlightingSystem : public GameSystem<CodeFile, TextEditorColorer, TextEditor> {
    public:
        void Initialize();
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        void Update(float dt);
    private:
        void LinesChanged(GameObject* object);
        void TranslationUnitUpdated(GameObject* object);
        void TextInserted(Point offset, GameObject* object);
        Worker worker;
        std::set<GameObject*> workingObjects;
    
    public:
        struct Token {
            unsigned startLine;
            unsigned startColumn;
            unsigned endLine;
            unsigned endColumn;
            
            std::string filename;
            std::string cursorSpelling;
            
            Colour color;
        };
    
        using Tokens = std::vector<Token>;
};
