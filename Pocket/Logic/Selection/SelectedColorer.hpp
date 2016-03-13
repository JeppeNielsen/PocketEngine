//
//  SelectedColorer.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 03/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Colour.hpp"

namespace Pocket {
    class SelectedColorer {
    public:
        SelectedColorer();
        Colour Deselected;
        Colour Selected;
    };
}