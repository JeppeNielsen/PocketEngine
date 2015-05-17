//
//  SerializedProperty.cpp
//  Editor
//
//  Created by Jeppe Nielsen on 11/13/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "SerializedProperty.hpp"

using namespace Pocket;

ISerializedProperty::ISerializedProperty() : parent(0) {}

ISerializedProperty::~ISerializedProperty() {
    while (children.size()>0) {
        delete children.back();
        children.pop_back();
    }
}


ISerializedProperty* ISerializedProperty::GetParent() { return parent; }
const ISerializedProperty::Children& ISerializedProperty::GetChildren() { return children; }

ISerializedPropertyEditor* ISerializedProperty::GetEditor() {
    return 0;
}

ISerializedProperty* ISerializedProperty::Get(std::string name) {
    for (size_t i=0; i<children.size(); i++) {
        if (children[i]->name == name) {
            return children[i];
        }
    }
    return 0;
}
