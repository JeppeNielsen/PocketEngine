//
//  FieldEditor.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 24/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"

namespace Pocket {
    Component(FieldEditor)
        public:
            FieldEditor();
            void Reset();
            ISerializedFieldEditor* editor;
            Property<FieldEditor*, ISerializable*> Object;
            Property<FieldEditor*, std::string> Field;
    };
}