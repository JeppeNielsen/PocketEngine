//
//  ProjectSettings.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 21/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include "TypeInfo.hpp"
#include "GameObjectHandle.hpp"
#include <vector>

using namespace Pocket;

struct ProjectSettings {
    std::string name;
    GameObjectHandle startupScene;
    std::string bundleIdentifier;
    GameObjectHandle icon;
    
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(name)
    TYPE_FIELD(startupScene)
    TYPE_FIELD(bundleIdentifier)
    TYPE_FIELD(icon)
    TYPE_FIELDS_END
};
