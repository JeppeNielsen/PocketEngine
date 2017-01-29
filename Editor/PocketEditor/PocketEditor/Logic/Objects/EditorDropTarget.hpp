//
//  EditorDropTarget.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 26/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <functional>
#include "EditorObject.hpp"

struct EditorDropTarget {
    std::function<void(EditorObject* editorObject)> OnDropped;
};
