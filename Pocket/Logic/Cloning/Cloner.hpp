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
#include "GameObjectHandle.hpp"
#include "TypeInfo.hpp"

namespace Pocket {
    class Cloner {
    public:
        
        GameObjectHandle Source;
        
        std::vector<std::shared_ptr<IFieldInfo>> variables;
        std::vector<ComponentId> components;
        
        void TryStoreVariables();
        
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(Source)
        TYPE_FIELD(variables)
        TYPE_FIELDS_END        
    };
}
