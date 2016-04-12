//
//  EditorSelection.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 06/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameObject.hpp"

using namespace Pocket;

struct EditorSelection {
    EditorSelection();
    GameObject* selectionObject;
};