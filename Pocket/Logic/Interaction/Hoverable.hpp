//
//  Hoverable.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 07/10/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Touchable.hpp"

namespace Pocket {
    class Hoverable {
    public:
        Event<TouchData> Enter;
        Event<TouchData> Exit;
    };
}
