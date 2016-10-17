//
//  Cloner.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 17/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <vector>
#include "Property.hpp"
#include "TypeInfo.hpp"

namespace Pocket {
    class Cloner {
    public:
        Property<std::string> SourceID;
        std::vector<int> storedComponents;
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(SourceID)
        TYPE_FIELDS_END
    };
}