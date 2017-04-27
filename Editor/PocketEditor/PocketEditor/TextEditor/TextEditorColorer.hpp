//
//  TextEditorColorer.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 23/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Colour.hpp"
#include <vector>

using namespace Pocket;

struct TextEditorColorer {
    struct ColorRange {
        int start;
        int end;
        Colour color;
    };
    
    void AddColor(int start, int end, Colour color);
    Colour FindColor(int index);
    void MoveNodes(int start, int offset);
    
    std::vector<ColorRange> colors;
};
