//
//  TextEditor.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 28/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include <string>
#include "DirtyProperty.hpp"
#include "Point.hpp"

using namespace Pocket;

struct TextEditor {

    TextEditor();

    std::string text;
    
    Property<int> Cursor;
    Property<bool> SelectionActive;
    Property<Point> Selection;
    
    Property<bool> AutoCompleteActive;
    Event<> AutoCompleteEnabled;
    Event<> AutoCompleteDisabled;
    int AutoCompleteCursor;
    
    enum class AutoCompleteEventType {
        NextToken,
        PrevToken,
        TokenSelected
    };
    Event<AutoCompleteEventType> AutoCompleteEvent;
    
    struct Line {
        std::string::size_type start;
        std::string::size_type end;
    };
    
    using LineList = std::vector<Line>;
    DirtyProperty<LineList> Lines;
    
    Point CursorToCartesian(int cursor);
    int CartesianToCursor(Point coord);
    
    void MoveCursor(Point dir);
    
    void MoveSelection(Point dir);
    
    void EraseText(int start, int end);
    void InsertText(int position, const std::string& text);
    
    bool IsCursorAtWhiteSpace();
    bool IsCursorAtToken(const std::string& token);
    
    void EnableAutoComplete(int cursor);
    void DisableAutoComplete();
    
    std::string GetAutoCompleteString();
};
