//
//  HierarchyEditor.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 02/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "GameWorld.hpp"

namespace Pocket {
  Component(HierarchyEditor)
  public:
        HierarchyEditor();
        void Reset();
        GameObject* Object;
    
        size_t prevChildrenCount;
  };
}
