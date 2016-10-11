//
//  InputController.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 10/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <vector>
#include "Event.hpp"
#include "TypeInfo.hpp"

namespace Pocket {
    class InputController {
    public:
        using ButtonList = std::vector<std::string>;
        ButtonList downButtons;
        ButtonList previouslyDownButtons;

        Event<std::string> ButtonDown;
        Event<std::string> ButtonUp;
        
        bool IsButtonDown(std::string button);
        
        void InvokeEvents();
        
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(downButtons)
        TYPE_FIELDS_END
    };
}