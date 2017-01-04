//
//  CloneVariable.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 20/11/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include "TypeInfo.hpp"

namespace Pocket {
    class CloneVariable {
    public:
        struct Variable {
            std::string componentName;
            std::string fieldName;
            
            TYPE_FIELDS_BEGIN
            TYPE_FIELD(componentName)
            TYPE_FIELD(fieldName)
            TYPE_FIELDS_END
        };
        
        std::vector<Variable> Variables;
        
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(Variables)
        TYPE_FIELDS_END
    };
}