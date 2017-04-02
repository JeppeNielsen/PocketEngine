//
//  TextEditorRenderer.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 28/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include "Vector2.hpp"

using namespace Pocket;

struct TextEditorRenderer {
    float fontSize;
    Property<Vector2> Offset;
};
