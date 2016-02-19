//
//  Orderable.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/29/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"

namespace Pocket {
    struct Orderable {
        Orderable() { Order = 0; }
        Property<int> Order;
    
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(Order)
        TYPE_FIELDS_END
    };
}