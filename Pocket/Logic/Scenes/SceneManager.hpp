//
//  SceneLayer.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 25/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameObjectHandle.hpp"
#include "Property.hpp"
#include <map>

namespace Pocket {
    class SceneManager {
    public:
        Property<std::map<int, GameObjectHandle>> ActiveScenes;
        Property<std::map<int, GameObjectHandle>> SetScenes;
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(SetScenes)
        TYPE_FIELDS_END
    };
}
