//
//  PanelArea.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 04/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "PanelArea.hpp"

using namespace Pocket;

PanelArea::PanelArea() { IsDirty = false; }

float PanelArea::GetSplitValue(const std::string& id) {
    auto it = Splits.find(id);
    return it!=Splits.end() ? it->second : 0.5f;
}

void PanelArea::SetSplitValue(const std::string& id, float value) {
    Splits[id] = value;
    SplitValueChanged(id);
}
