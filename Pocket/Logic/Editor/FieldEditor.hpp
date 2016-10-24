//
//  FieldEditor.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 24/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "TypeInfo.hpp"

namespace Pocket {
    struct FieldEditor {
        public:
            FieldEditor();
            void operator=(const FieldEditor& other);
            IFieldInfoEditor* editor;
            Property<std::string> Field;
            TypeInfo Type;
            void SetType(const TypeInfo& type);
            Event<> TypeChanged;
    };
}