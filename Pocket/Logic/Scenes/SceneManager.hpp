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

        std::map<int, GameObject*> sceneRoots;

        TYPE_FIELDS_BEGIN
        TYPE_FIELD(ActiveScenes)
        TYPE_FIELDS_END
    };
}
