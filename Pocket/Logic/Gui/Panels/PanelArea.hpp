//
//  PanelArea.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 04/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <map>
#include <string>
#include "Event.hpp"

namespace Pocket {
    struct PanelArea {
        PanelArea();
        float GetSplitValue(const std::string& id);
        void SetSplitValue(const std::string& id, float value);
        
        std::map<std::string, float> Splits;
        bool IsDirty;
        Event<std::string> SplitValueChanged;
    };
}
