//
//  AtlasCreator.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 27/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <map>
#include <string>
#include "Box.hpp"

class AtlasCreator {
public:

    using Offsets = std::map<std::string, Pocket::Box>;

    bool CreateAtlas(const std::string& inputPath, const std::string& texturePath, const std::string& atlasPath, int maxWidth, int maxHeight, const Offsets& offsets);
};
