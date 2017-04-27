//
//  TextEditorColorer.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 23/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "TextEditorColorer.hpp"


void TextEditorColorer::AddColor(int start, int end, Pocket::Colour color) {
    colors.push_back({
        start, end, color
    });
    
    std::sort(colors.begin(), colors.end(), [] (const ColorRange& a, const ColorRange& b) {
        return (a.start>b.start);
    });
}

Colour TextEditorColorer::FindColor(int index) {
    int low = 0;
    int high = (int)colors.size() - 1;
    
    while (low<=high) {
        int mid = (low + high) / 2;
        
        auto& color = colors[mid];
        if (index>=color.start && index<color.end) {
            return color.color;
        } else if (index>=color.start) {
            high = mid - 1;
        } else if (index<color.start) {
            low = mid + 1;
        }
    }
    
    return Colour::White();
}

void TextEditorColorer::MoveNodes(int start, int offset) {
    for(auto& r : colors) {
        if (r.start<start) {
            continue;
        } else {
            r.start+=offset;
            r.end+=offset;
        }
    }
}
