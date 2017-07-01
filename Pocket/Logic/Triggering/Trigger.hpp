//
//  Trigger.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 25/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include "GameObjectHandle.hpp"
#include "TypeInfo.hpp"

namespace Pocket {
    class Trigger {
    public:
        Trigger();
        
        GameObjectHandle Source;
        std::vector<std::shared_ptr<IFieldInfo>> variables;
        
        void CreateVariables(GameObject* root);
        void Invoke();
        
        GameObject* clone;
        
        void TryStoreVariables();
        
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(Source)
        TYPE_FIELD(variables)
        TYPE_FIELDS_END        
    
    private:
        void FindVariables(std::vector<std::shared_ptr<IFieldInfo>>& variables, GameObject* objectWithVariable);
    };
}
