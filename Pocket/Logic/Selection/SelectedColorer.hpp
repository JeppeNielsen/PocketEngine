//
//  SelectedColorer.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 03/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"

namespace Pocket {
  Component(SelectedColorer)
    public:
        Colour Deselected;
        Colour Selected;
        void Reset();
    };
}