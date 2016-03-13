//
//  SelectedColorer.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 03/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "SelectedColorer.hpp"

using namespace Pocket;

SelectedColorer::SelectedColorer() {
    Deselected = Colour::White();
    Selected = Colour::White();
}