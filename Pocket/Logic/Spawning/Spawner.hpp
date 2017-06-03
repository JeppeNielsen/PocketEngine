//
//  Spawner.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 02/06/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <vector>
#include "GameObjectHandle.hpp"
#include "TypeInfo.hpp"

namespace Pocket {
    class Spawner {
    public:
        GameObjectHandle Source;
        Property<int> Spawn;
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(Source)
        TYPE_FIELD(Spawn)
        TYPE_FIELDS_END
    };
}
