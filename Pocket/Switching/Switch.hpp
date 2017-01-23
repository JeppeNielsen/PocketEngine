//
//  Switch.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 21/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include "TypeInfo.hpp"

namespace Pocket {
    struct Switch {
        Switch();
        Property<bool> On;
        Property<int> Group;
        
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(On);
        TYPE_FIELD(Group)
        TYPE_FIELDS_END
    };
}
