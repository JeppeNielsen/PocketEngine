//
//  Colorable.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/27/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "Colorable.hpp"

using namespace Pocket;

Colorable::Colorable() : Color(this) {}

void Colorable::Reset() {
    Color = Colour::White();
}


void Colorable::Serialize(Pocket::ISerializedProperty *property) {
    property->Add("Color", Color());
}

void Colorable::Deserialize(Pocket::ISerializedProperty *property) {
    Color = property->GetValue("Color", Colour::White());
}
