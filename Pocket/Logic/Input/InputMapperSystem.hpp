//
//  InputMapperSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 10/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "InputMapper.hpp"
#include "InputController.hpp"
#include <vector>

namespace Pocket {
    class InputMapperSystem : public GameSystem<InputMapper, InputController> {
    public:
        void Initialize();
        void Update(float dt);
    private:
        void ButtonDown(ButtonEvent e);
        void ButtonUp(ButtonEvent e);
        
        using Buttons = std::vector<std::string>;
        Buttons buttons;
    };
}
