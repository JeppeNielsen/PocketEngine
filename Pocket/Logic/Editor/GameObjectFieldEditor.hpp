//
//  GameObjectField.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 21/10/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <functional>
#include "GameObject.hpp"

namespace Pocket {
    class GameObjectFieldEditor {
        public:
            std::function<void(GameObject*)> SetObject;
    };
}
