//
//  HierarchyEditor.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 02/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "GameObject.hpp"

namespace Pocket {
  class HierarchyEditor {
  public:
        HierarchyEditor();
        Property<GameObject*> Object;
        size_t prevChildrenCount;
  };
}
