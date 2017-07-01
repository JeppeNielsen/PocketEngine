//
//  ClonerSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 17/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Cloner.hpp"

namespace Pocket {
    class ClonerSystem : public GameSystem<Cloner> {
    protected:
        void Initialize();
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
    private:
        void CloneSourceChanged(GameObject* object);
        
        void FindVariables(std::vector<std::shared_ptr<IFieldInfo>>& variables, GameObject* cloner, GameObject* objectWithVariable);
        
    };
}
