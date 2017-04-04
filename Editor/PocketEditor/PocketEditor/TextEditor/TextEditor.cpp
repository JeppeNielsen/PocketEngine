//
//  TextEditor.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 28/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "TextEditor.hpp"

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
    Point p = CursorToCartesian(s.y);
    p += dir;
    s.y = CartesianToCursor(p);
    
    Selection = s;
}
