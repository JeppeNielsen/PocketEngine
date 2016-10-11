//
//  InputMapper.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 10/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <map>
#include <string>
#include "TypeInfo.hpp"

namespace Pocket {
    struct InputMapper {
        InputMapper();
        
        using ButtonMap = std::map<std::string, std::string>;
        ButtonMap map;
    
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(map)
        TYPE_FIELDS_END
    };
}