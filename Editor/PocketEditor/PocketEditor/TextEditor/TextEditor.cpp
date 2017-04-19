//
//  TextEditor.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 28/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "TextEditor.hpp"
#include "MathHelper.hpp"

TextEditor::TextEditor() {
    Cursor = 0;
    Lines.Method = [this] (LineList& value) {
        value.clear();
        std::string::size_type prev_pos = 0, pos = 0;
        while((pos = text.find("\n", pos)) != std::string::npos) {
            value.push_back({ prev_pos, pos });
            prev_pos = ++pos;
        }
        if (pos!=std::string::npos) {
            value.push_back({ prev_pos, pos });
        }
    };
    AutoCompleteActive = false;
}

Point TextEditor::CursorToCartesian(int cursor) {
    if (cursor<0) cursor = 0;
    auto& lines = Lines();
    if (lines.empty()) return Point(0,0);
    Point coord;
    coord.y = 0;
    for(auto& l : lines) {
        if (cursor<=l.end) {
            coord.x = cursor - (int)l.start;
            return coord;
        }
        coord.y++;
    }
    return Point((int)(lines.back().end - lines.back().start), (int)lines.size()-1);
}

int TextEditor::CartesianToCursor(Point coord) {
    auto& lines = Lines();
    if (lines.empty()) return 0;
    if (coord.y<0) {
        coord.y = 0;
    } else if (coord.y>=lines.size()) {
        coord.y = (int)lines.size()-1;
    }
    auto& line = lines[coord.y];
    if (coord.x<0) coord.x = 0;
    int maxX = (int)(line.end-line.start);
    if (coord.x<0) {
        coord.x = 0;
    } else if (coord.x>maxX) {
        coord.x = maxX;
    }
    int bla = (int)line.start + coord.x;
    if (bla<0) {
        return bla;
    }
    return bla;
}

void TextEditor::MoveCursor(Pocket::Point dir) {
    if (dir.y == 0) {
        Cursor = MathHelper::Clamp(Cursor() + dir.x, 0, text.size());
        return;
    }

    Point p = CursorToCartesian(Cursor);
    p += dir;
    Cursor = CartesianToCursor(p);
}

void TextEditor::MoveSelection(Pocket::Point dir) {
    if (!SelectionActive()) {
        SelectionActive = true;
        Selection = { Cursor, Cursor };
    }
    Point s = Selection;
    
    if (dir.y == 0) {
        s.y = MathHelper::Clamp(s.y + dir.x, 0, text.size());
    } else {
        Point p = CursorToCartesian(s.y);
        p += dir;
        s.y = CartesianToCursor(p);
    }
    
    Selection = s;
}

void TextEditor::InsertText(int position, const std::string &text) {
    this->text = this->text.insert(position, text);
    Lines.MakeDirty();
}

void TextEditor::EraseText(int start, int end) {
    this->text.erase(start, end - start);
}

bool TextEditor::IsCursorAtWhiteSpace() {
    if (Cursor()<1 || Cursor()>=text.size()) return false;
    char& ch = text[Cursor - 1];
    return (ch == '\n') || (ch == ' ');
}

bool TextEditor::IsCursorAtToken(const std::string &token) {
    if (Cursor()<0 || Cursor()>=text.size()) return false;
    std::string t = text.substr(Cursor() - token.size(), token.size());
    return t == token;
}

void TextEditor::EnableAutoComplete(int cursor) {
    AutoCompleteActive = true;
    AutoCompleteCursor = cursor;
    AutoCompleteEnabled();
}

void TextEditor::DisableAutoComplete() {
    AutoCompleteActive = false;
    AutoCompleteDisabled();
}

std::string TextEditor::GetAutoCompleteString() {
    int size = Cursor - AutoCompleteCursor;
    if (size<1) return "";
    return text.substr(AutoCompleteCursor, size);
}
